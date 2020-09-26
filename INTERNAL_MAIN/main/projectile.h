#define oNew 0x238240
#define oPoolGetTraceAll 0x315B00 //                          E8 ?? ?? ?? ?? 45 33 C9 48 89 45 67
#define vClientInputIndex 0x970

DWORD64 retProj = 0;
class Projectile;

bool PLOS(Vector3 a, Vector3 b) {
	typedef bool(__stdcall* LOS)(Vector3, Vector3, int, float);
	return ((LOS)(g_Base + O::GamePhysics::LineOfSight))(a, b, 2162688, 0.f);
}
bool PLOS(Vector3 a, Vector3 b, Vector3 c) {
	return PLOS(a, b) && PLOS(b, c);
}

Vector3 SimulateProjectile(Vector3& position, Vector3& velocity, float& partialTime, float travelTime, Vector3 gravity, float drag)
{
	float num = 0.03125f;
	Vector3 origin = position;
	if (partialTime > 0.f)
	{
		float num2 = num - partialTime;
		if (travelTime < num2)
		{
			origin = position;
			position += velocity * travelTime;
			partialTime += travelTime;
			return origin;
		}
		origin = position;
		position += velocity * num2;
		velocity += gravity * num;
		velocity -= velocity * drag * num;
		travelTime -= num2;
	}

	typedef int(__stdcall* FloorInt)(float);
	int num3 = ((FloorInt)(g_Base + O::UnityEngine_Mathf::FloorToInt))(travelTime / num);

	for (int i = 0; i < num3; i++)
	{
		origin = position;
		position += velocity * num;
		velocity += gravity * num;
		velocity -= velocity * drag * num;
	}
	partialTime = travelTime - num * (float)num3;
	if (partialTime > 0.f)
	{
		origin = position;
		position += velocity * partialTime;
	}
	return origin;
}



class Projectile {
public:

	void initialVelocity(Vector3 d) { safe_write(this + O::Projectile::initialVelocity, d, Vector3); }
	Vector3 initialVelocity() { return safe_read(this + O::Projectile::initialVelocity, Vector3); }

	Vector3 sentPosition() { return safe_read(this + O::Projectile::sentPosition, Vector3); }
	void sentPosition(Vector3 d) { safe_write(this + O::Projectile::sentPosition, d, Vector3); }

	void currentVelocity(Vector3 d) { safe_write(this + O::Projectile::currentVelocity, d, Vector3); }
	Vector3 currentVelocity() { return safe_read(this + O::Projectile::currentVelocity, Vector3); }

	float drag() { return safe_read(this + O::Projectile::drag, float); }

	float previoustraveledTime() { return safe_read(this + O::Projectile::previousTraveledTime, float); }
	void previoustraveledTime(float d) { safe_write(this + O::Projectile::previousTraveledTime, d, float); }

	float gravityModifier() { return safe_read(this + O::Projectile::gravityModifier, float); }
	void integrity(float f) { safe_write(this + O::Projectile::integrity, f, float); }
	float integrity() { return safe_read(this + O::Projectile::integrity, float); }
	float maxDistance() { return safe_read(this + O::Projectile::maxDistance, float); }
	Vector3 currentPosition() { return safe_read(this + O::Projectile::currentPosition, Vector3); }
	void currentPosition(Vector3 d) { safe_write(this + O::Projectile::currentPosition, d, Vector3); }

	void partialTime(float f) { safe_write(this + O::Projectile::tumbleSpeed, f, float); }
	float partialTime() { return safe_read(this + O::Projectile::tumbleSpeed, float); }
	Vector3 prevSentVelocity() { return safe_read(this + O::Projectile::tumbleAxis, Vector3); }
	void prevSentVelocity(Vector3 d) { safe_write(this + O::Projectile::tumbleAxis, d, Vector3); }
	float sentTraveledTime() { return safe_read(this + O::Projectile::closeFlybyDistance, float); }
	void sentTraveledTime(float d) { safe_write(this + O::Projectile::closeFlybyDistance, d, float); }
	float lastUpdateTime() { return safe_read(this + O::Projectile::ricochetChance, float); }
	void lastUpdateTime(float d) { safe_write(this + O::Projectile::ricochetChance, d, float); }
	Vector3 prevSentPosition() { return safe_read(this + O::Projectile::swimScale, Vector3); }
	void prevSentPosition(Vector3 d) { safe_write(this + O::Projectile::swimScale, d, Vector3); }
	bool needsLOS() { return safe_read(this + O::Projectile::createDecals, bool); }
	void needsLOS(bool d) { safe_write(this + O::Projectile::createDecals, d, bool); }

	float traveledDistance() { return safe_read(this + O::Projectile::traveledDistance, float); }
	void traveledDistance(float d) { safe_write(this + O::Projectile::traveledDistance, d, float); }

	float initialDistance() { return safe_read(this + O::Projectile::initialDistance, float); }
	
	float traveledTime() { return safe_read(this + O::Projectile::traveledTime, float); }
	void traveledTime(float d) { safe_write(this + O::Projectile::traveledTime, d, float); }

	Vector3 previousPosition() { return safe_read(this + O::Projectile::previousPosition, Vector3); }
	void previousPosition(Vector3 d) { safe_write(this + O::Projectile::previousPosition, d, Vector3); }

	void hitTest(DWORD64 d) { safe_write(this + O::Projectile::hitTest, d, DWORD64); }
	DWORD64 hitTest() { return safe_read(this + O::Projectile::hitTest, DWORD64); }
	DWORD64 damageProperties() { return safe_read(this + O::Projectile::damageProperties, DWORD64); }

	int projectileID() { return safe_read(this + O::Projectile::projectileID, int); }

	BasePlayer* owner() { return (BasePlayer*)safe_read(this + O::Projectile::owner, DWORD64); }
	DWORD64 sourceWeaponPrefab() { return safe_read(this + O::Projectile::sourceWeaponPrefab, DWORD64); }

	DWORD64 damageDistances() { return safe_read(this + O::Projectile::damageDistances, DWORD64); }

	bool invisible() { return safe_read(this + O::Projectile::invisible, bool); }

	float launchTime() { return safe_read(this + O::Projectile::launchTime, float); }
	void launchTime(float d) { return safe_write(this + O::Projectile::launchTime, d, float); }

	bool IsAlive() {
		return (this->integrity() > 0.001f && this->traveledDistance() < this->maxDistance() && this->traveledTime() < 8.f);
	}

	void UpdateHitTest(TraceInfo info) {
		DWORD64 ht = hitTest();

		safe_write(ht + 0x66, true, bool); //DidHit
		safe_write(ht + 0x88, info.entity, DWORD64); //HitEntity
		safe_write(ht + 0xA8, info.distance, float); //HitDistance
		//

		typedef pUncStr(__stdcall* GetName)(DWORD64);
		if (info.material != 0) {
			pUncStr material = (pUncStr)(((GetName)(g_Base + O::AssetNameCache::GetName))(info.material));
			safe_write(ht + 0xC0, material, pUncStr); //HitMaterial  
		}
		//else safe_write(ht + 0xC0, Str(xorstr(L"generic")), Str);

		/*string GetName(PhysicMaterial mat);*/
//printf("hit material %S\n", material.str);
		safe_write(ht + 0xB8, info.partID, unsigned int); //HitPart
		safe_write(ht + 0xB0, info.bone, DWORD64); //HitTransform
		safe_write(ht + 0x90, info.point, Vector3); //HitPoint
		safe_write(ht + 0x9C, info.normal, Vector3); //HitNormal
		safe_write(ht + 0x78, info.collider, DWORD64); //collider

		typedef DWORD64(__stdcall* Ret)(DWORD64);
																	   //GameObject get_gameObject();
		DWORD64 go = info.collider != 0 ? ((Ret)(g_Base + O::UnityEngine_Component::get_gameObject))(info.collider) : ((Ret)(g_Base + O::UnityEngine_Component::get_gameObject))(info.bone);
		safe_write(ht + 0x70, go, DWORD64); //gameObject
		if (info.bone != 0) {
			typedef Vector3(__stdcall* ITP)(DWORD64, Vector3);
			Vector3 hitpoint = ((ITP)(g_Base + O::UnityEngine_Transform::InverseTransformPoint))(safe_read(ht + 0xB0, DWORD64)/*HitTransform*/, info.point);
			safe_write(ht + 0x90, hitpoint, Vector3); //hitPoint

			Vector3 normalpoint = ((ITP)(g_Base + O::UnityEngine_Transform::InverseTransformDirection))(safe_read(ht + 0xB0, DWORD64)/*HitTransform*/, info.normal);
			safe_write(ht + 0x9C, normalpoint, Vector3); //HitNormal
		}

	}

	bool DoFatBulletHit(Projectile* pr, Vector3 point) {
		if (!Vars::Weapon::FatBullet) return false;

		float maxdist = GetHitDist();

		//Vector3 _point = FindVisiblePoint(sentPosition(), point);

		AimResult target = FindAimTarget(point, false, false, maxdist, 0.f, true);

		if (IsAlive() && target.valid) {
			if (!PLOS(target.pos, point)) {
				return false;
			}

			DWORD64 ht = hitTest();
			safe_write(ht + 0x66, true, bool); //DidHit
			safe_write(ht + 0x88, (DWORD64)target.entity, DWORD64); //HitEntity
			

			Str st = Str(L"");
			if (Vars::Aim::AimHead) {
				st = Str(xorstr(L"head"));
			}
			else if(random() % 4 < 2){
				st = Str(xorstr(L"spine4"));
			}
			else {
				st = Str(xorstr(L"spine1"));
			}
			typedef DWORD64(__stdcall* FindBone)(DWORD64, Str);

			DWORD64 Transform = ((FindBone)(g_Base + O::BaseEntity::FindBone))((DWORD64)target.entity, st);

			if (!Transform) return false;
			safe_write(ht + 0xB0, Transform, DWORD64);

			typedef Vector3(__stdcall* ITP)(DWORD64, Vector3);
			Vector3 hitpoint = ((ITP)(g_Base + O::UnityEngine_Transform::InverseTransformPoint))(Transform, point);
			safe_write(ht + 0x90, hitpoint, Vector3); //hitPoint
			Vector3 attackStart = Simulate(false, true);
			safe_write(ht + 0x14, attackStart, Vector3);

			float org;
			if (Vars::Aim::FakeDist) {
				org = traveledDistance(); float real = 666.f + random() % 200;
				traveledDistance(real);
			}

			typedef bool(__stdcall* Hit)(Projectile*, DWORD64, Vector3, Vector3);
			bool result = ((Hit)(g_Base + O::Projectile::DoHit))(pr, ht, point, Vector3());
			typedef void(__stdcall* Sphere)(Vector3, float, Color, float, bool);
			((Sphere)(g_Base + O::UnityEngine_DDraw::Sphere))(point, 0.015f, Color(1, 0, 0, 1), 20.f, true);

			if (Vars::Aim::FakeDist) {
				traveledDistance(org);
			}
			return true;
		}
		return false;
	}

	bool DoHit(Projectile* pr, DWORD64 ht, Vector3 point, Vector3 normal, TraceInfo info, bool& exit) {
		bool result = false;
		if (!IsAlive()) return result;

		//penetration power 2.f, integrity 1.f
		typedef pUncStr(__stdcall* Name)(DWORD64);

		typedef pUncStr(__stdcall* GetName)(DWORD64);
		Str material = info.material != 0 ? Str(((GetName)(g_Base + O::AssetNameCache::GetName))(info.material)->str) : Str(xorstr(L"generic"));
		/*string GetName(PhysicMaterial mat);*/
		

		
		typedef void(__stdcall* SetName)(DWORD64, Str);
		typedef bool(__stdcall* Hit)(Projectile*, DWORD64, Vector3, Vector3);


		//FindVisiblePoint(sentPosition(), point);

		bool canIgnore = PLOS(sentPosition(), currentPosition() + currentVelocity().Normalized() * 0.01f);
		if (!canIgnore) {
			integrity(0.f); return true;
		}

		float org;
		if (Vars::Aim::FakeDist) {
			org = traveledDistance(); float real = 666.f + random() % 200;
			traveledDistance(real);
		}

		if (canIgnore) {
			Vector3 attackStart = Simulate(false, true);

			safe_write(ht + 0x14, Ray(attackStart, Vector3()), Ray);
		}


		if (canIgnore && m_wcsicmp(material.str, xorstr(L"Flesh"))) {
			DWORD64 Tra = safe_read(ht + 0xB0, DWORD64);
			if (Tra && random() % 3 != 0 && (Vars::Aim::AimHead || Vars::Aim::AimBody)) {
				Str st = Str(L"");
				/*if (Vars::Aim::Bone == AimBone::head) st = Str(xorstr(L"head"));
				else if (Vars::Aim::Bone == AimBone::neck) st = Str(xorstr(L"neck"));
				else if (Vars::Aim::Bone == AimBone::pelvis) st = Str(xorstr(L"pelvis"));
				else if (Vars::Aim::Bone == AimBone::spine) st = Str(xorstr(L"spine4"));
				else if (Vars::Aim::Bone == AimBone::leg) st = Str(xorstr(L"l_toe"));*/
				if (Vars::Aim::AimHead) {
					st = Str(xorstr(L"head"));
				}
				else {
					st = Str(xorstr(L"spine4"));
				}

				//Object set_name
				((SetName)(g_Base + O::UnityEngine_Object::set_name))(Tra, st);
			}
			

			result = ((Hit)(g_Base + O::Projectile::DoHit))(pr, ht, point, normal);
			sentPosition(currentPosition());

		}
		
		if (Vars::Aim::FakeDist) {
			traveledDistance(org);
		}
		return result;
	}

	bool DoMovement(float deltaTime, Projectile* pr) {
		//swim
		/*if (!(swimScale() == Vector3())) {
			typedef float(__stdcall* RetF)();
			float time = ((RetF)oTime)();
			float num = time + swimRandom();
			Vector3 speed = swimSpeed();
			Vector3 scale = swimScale();

			Vector3 vector = Vector3(m_sin(num * speed.x) * scale.x, m_cos(num * speed.y) * scale.y, m_sin(num * speed.z) * scale.z);

			typedef DWORD64(__stdcall* Ret)(DWORD64);
			DWORD64 Transform = ((Ret)oGetTr)((DWORD64)pr);

			typedef Vector3(__stdcall* ITP)(DWORD64, Vector3);
			Vector3 inversed = ((ITP)oInverseTransformDirection)(Transform, vector);
			currentVelocity(currentVelocity() + inversed);
		}*/
		
		Vector3 a = currentVelocity() * deltaTime;
		float magnitude = a.Length();
		float num2 = 1.f / magnitude;
		Vector3 vec2 = a * num2;
		bool flag = false;

		Vector3 vec3 = currentPosition() + vec2 * magnitude;
		float num3 = traveledTime() + deltaTime;

		DWORD64 ht = hitTest();
		if (!ht) {
			DWORD64 htstatic = safe_read(GetModBase() + S::HitTest, DWORD64);

			DWORD64 New = GetModBase() + oNew;
			typedef DWORD64(__stdcall* NNew)(DWORD64);

			DWORD64 HitTest = ((NNew)New)(htstatic);
			ht = HitTest;
			hitTest(HitTest);
		}
		Ray ray = Ray(currentPosition(), vec2);
		safe_write(ht + 0x14, ray, Ray); //AttackRay
		safe_write(ht + 0x34, magnitude, float); //MaxDistance

		BasePlayer* ow = this->owner();
		safe_write(ht + 0x80, (DWORD64)ow, DWORD64); //IgnoreEntity
		safe_write(ht + 0x2C, 0.f, float); //Radius
		safe_write(ht + 0x30, 0.15f, float); //Forgiveness                                        FAT BULLET
		if (!LocalPlayer || ow->GetSteamID() == LocalPlayer->GetSteamID()) {
			safe_write(ht + 0x10, 0x2, int); //Type
		}
		else safe_write(ht + 0x10, 0x1, int); //Type	`

		if (sourceWeaponPrefab()) {
			safe_write(ht + 0x65, true, bool); //BestHit
			safe_write(ht + 0x68, damageProperties(), DWORD64); //DamageProperties
		}

		typedef void(__stdcall* TraceAll)(DWORD64, DWORD64, int);
		typedef DWORD64(__stdcall* Unknown)(DWORD64);
		typedef void(__stdcall* UnknownV)(DWORD64, DWORD64);
		DWORD64 st = safe_read(GetModBase() + S::PoolGetListTraceInfo, DWORD64); //static GetList (.data)
		
		Unknown get_list = (Unknown)(GetModBase() + oPoolGetTraceAll); 

		DWORD64 rs = get_list(st);
		if (!rs) return false;

		((TraceAll)(g_Base + O::GameTrace::TraceAll))(ht, rs, 1269916433 /*mask*/);

		//printf("TraceAll() called\n");
		ht = hitTest();
		safe_write(ht + 0x34, 0.f, float); //AttackEnd == AttackStart
		

		int size = safe_read(rs + 0x18, int);

		DWORD64 lst = safe_read(rs + 0x10, DWORD64);

		//printf("list 0x%llX, size %d\n", rs, size);
		typedef Vector3(__stdcall* GetP)(DWORD64);

		

		if (DoFatBulletHit(pr, currentPosition())) {
			integrity(0.f); return false;
		}
		
		
		for (int i = 0; i < size && this->IsAlive() && !flag; i++) {
			TraceInfo Trace = safe_read(lst + 0x20 + (i * sizeof(TraceInfo)), TraceInfo);
			if (Trace.valid) {
				
				typedef void(__stdcall* UpdateT)(TraceInfo, DWORD64);
				//char* name = (char*)safe_read(safe_read(Trace.entity, DWORD64) + 0x10, DWORD64);

				//((UpdateT)(GetModBase() + 0xD58A0))(Trace, ht);
				this->UpdateHitTest(Trace); //оригинальный крашит, почему - хз UPD: что-то с PhysicMaterial

				Vector3 vec4 = ((GetP)(g_Base + O::HitTest::HitPointWorld))(ht); //Vector3 HitPointWorld();
				Vector3 normal = ((GetP)(g_Base + O::HitTest::HitNormalWorld))(ht); //Vector3 HitNormalWorld();

				if (Vars::Weapon::DebugDraw) {
					typedef void(__stdcall* Line)(Vector3, Vector3, Color, float, bool, bool);
					((Line)(g_Base + O::UnityEngine_DDraw::Line))(currentPosition(), vec4, Color(1, 1, 1, 1), 20.f, true, true);

					typedef void(__stdcall* Sphere)(Vector3, float, Color, float, bool);
					((Sphere)(g_Base + O::UnityEngine_DDraw::Sphere))(Trace.point, 0.05f, Color(0.5, 0, 0, 1), 20.f, true);
				}
				
				float magnitude2 = (vec4 - currentPosition()).Length();
				float num5 = magnitude2 * num2 * deltaTime;

				traveledDistance(traveledDistance() + magnitude2);
				traveledTime(traveledTime() + num5);
				currentPosition(vec4);

				bool exit = false;
				if (this->DoHit(pr, ht, vec4, normal, Trace, exit)) {
					flag = true;
				}
				
				if (exit) {
					return true;
				}
			}
		}

		if (!flag && this->IsAlive()) {
			if (Vars::Weapon::DebugDraw) {
				typedef void(__stdcall* Arrow)(Vector3, Vector3, float, Color, float);
				((Arrow)(g_Base + O::UnityEngine_DDraw::Arrow))(currentPosition(), vec3, 0.05f, Color(1, 1, 1, 1), 20.f);
			}
			
			float magnitude3 = (vec3 - currentPosition()).Length();
			float num6 = magnitude3 * num2 * deltaTime;
			traveledDistance(traveledDistance() + magnitude3);
			traveledTime(traveledTime() + num6);
			currentPosition(vec3);
		}

		return false;
	}

	void DoVelocityUpdate(float deltaTime, Projectile* pr) {
		//if you want Updates, just call original
		if (!IsAlive()) return;


		//typedef void(__stdcall* Upd)(Projectile*, float);
		//return ((Upd)oProjVelUpd)(pr, deltaTime);

		//typedef void(__stdcall* Upd)(BasePlayer*, PlayerUpdate);
		typedef Vector3(__stdcall* RetV)();
		Vector3 gr = ((RetV)(g_Base + O::UnityEngine_Physics::get_gravity))(); //static Vector3 get_gravity();

		Vector3 tr = (gr * gravityModifier() * deltaTime);
		currentVelocity(currentVelocity() + tr);

		Vector3 dr = (currentVelocity() * drag() * deltaTime);
		currentVelocity(currentVelocity() - dr);

		//PlayerUpdate update = PlayerUpdate(projectileID(), previousPosition(), previouxorstrveledTime());

		//((Upd)(GetModBase(L"GameAssembly.dll") + 0x28EA70))(owner(), update);

		//sentPosition(previousPosition());
		//send update
	}

	void UpdateVelocity(float deltaTime, Projectile* pr, bool& rett) {
		if (traveledTime() != 0.f) {
			previousPosition(this->currentPosition());
			previoustraveledTime(this->traveledTime());
		}
		typedef DWORD64(__stdcall* Ret)(DWORD64);
		DWORD64 Transform = ((Ret)(g_Base + O::UnityEngine_Component::get_transform))((DWORD64)pr);
		Vector3 pos = LocalPlayer->GetBone(Transform);
		this->currentPosition(pos);

		typedef float(__stdcall* RetFEmpty)();
		if (traveledTime() == 0.f) {
			this->sentPosition(pos);
			this->previousPosition(pos);
			partialTime(0.f); sentTraveledTime(0.f);  prevSentVelocity(initialVelocity()); prevSentPosition(sentPosition()); needsLOS(false);
		}
		
		deltaTime *= ((RetFEmpty)(g_Base + O::UnityEngine_Time::get_timeScale))();
		
		

		bool ret = this->DoMovement(deltaTime, pr);
		if (!ret) {
			this->DoVelocityUpdate(deltaTime, pr);
		}
		

		typedef void(__stdcall* SetP)(DWORD64, Vector3);
		typedef DWORD64(__stdcall* GetTr)(DWORD64);

		DWORD64 Trans = ((GetTr)(g_Base + O::UnityEngine_Component::get_transform))((DWORD64)pr); //Component | Transform get_transform(); 
		((SetP)(g_Base + O::UnityEngine_Transform::set_position))(Trans, currentPosition()); //Transform | void set_position(Vector3 value); 
		/*//rotate
		if (tumbleSpeed() > 0.f) {
			typedef void(__stdcall* RotateTr)(DWORD64, Vector3, float);
			((RotateTr)oTrRotate)(Trans, tumbleAxis(), tumbleSpeed() * deltaTime);
			//                                          void Rotate(Vector3 axis, float angle);
			return;
		}*/
		typedef void(__stdcall* SetQ)(DWORD64, Vector4);
		typedef Vector4(__stdcall* LookRotation)(Vector3, Vector3);

		Vector4 rotation = ((LookRotation)(g_Base + O::UnityEngine_Quaternion::LookRotation))(currentVelocity(), Vector3(0, 1, 0));

		((SetQ)(g_Base + O::UnityEngine_Transform::set_rotation))(Trans, rotation);
		//                            void set_rotation(Quaternion value);
	}

	void Launch(Projectile* pr) {
		if (Vars::Aim::Silent::Enabled && !Vars::User::Panic && (!LocalPlayer || (LocalPlayer && pr->owner()->GetSteamID() == LocalPlayer->GetSteamID()))) {
			bool ret = false;
			for (; pr->IsAlive(); pr->UpdateVelocity(0.03125f, pr, ret)) {
				if (initialDistance() <= traveledDistance() && traveledTime() >= 0.1f) {
					break;
				}
				if (ret) {
					break;
				}
			}
			typedef float(__stdcall* RetFEmpty)();
			float time = ((RetFEmpty)(g_Base + O::UnityEngine_Time::get_fixedTime))();
			time -= traveledTime();
			launchTime(time);
		}
		else {
			typedef void(__stdcall* L)(DWORD64);
			return ((L)(g_Base + O::Projectile::Launch))((DWORD64)pr);
		}
	}

	/*static void CheckBullet(Projectile* bullet) { //USELESS
		const DWORD64 local = LocalPlayer->GetSteamID();
		const float delta = 0.03125f;
		const float drag = bullet->drag();
		Vector3 velocity = bullet->initialVelocity();
		typedef DWORD64(__stdcall* Ret)(DWORD64);
		DWORD64 Transform = ((Ret)oGetTr)((DWORD64)bullet);
		Vector3 position = LocalPlayer->GetBone(Transform);
		Vector3 origin = position;

		DWORD64 htstatic = safe_read(GetModBase() + oHitTestStatic, DWORD64);

		DWORD64 New = GetModBase() + oNew;
		typedef DWORD64(__stdcall* NNew)(DWORD64);
		DWORD64 ht = ((NNew)New)(htstatic);


		typedef int(__stdcall* FloorInt)(float);
		int num3 = ((FloorInt)oFloorInt)(0.5f / delta);

		Vector3 lpVel = LocalPlayer->GetVelocity();
		for (int i = 0; i < num3; i++)
		{
			origin = position;
			position += velocity * delta;
			velocity += Vector3(0, -9.1f, 0) * delta;
			velocity -= velocity * drag * delta;

			float dist = (position - origin).Length();
			Vector3 norm = (position - origin) / dist; //optimization lvl 80

			safe_write(ht + 0x14, Ray(origin, norm), Ray); //AttackRay
			safe_write(ht + 0x34, dist, float); //MaxDistance
			safe_write(ht + 0x2C, 0.f, float); //Radius
			safe_write(ht + 0x10, 0x2, int); //Type

			typedef void(__stdcall* TraceAll)(DWORD64, DWORD64, int);
			typedef DWORD64(__stdcall* Unknown)(DWORD64);
			DWORD64 st = safe_read(GetModBase() + oGetListStatic, DWORD64); //static GetList (.data)

			Unknown get_list = (Unknown)(GetModBase() + oPoolGetTraceAll);

			DWORD64 rs = get_list(st);
			if (!rs) return;
			((TraceAll)oTraceAll)(ht, rs, 131072);

			int size = safe_read(rs + 0x18, int);
			DWORD64 lst = safe_read(rs + 0x10, DWORD64);
			for(int i = 0; i < size; i++){
				TraceInfo Trace = safe_read(lst + 0x20 + (i * sizeof(TraceInfo)), TraceInfo);
				if (!Trace.entity) continue;
				if (((BasePlayer*)Trace.entity)->GetSteamID() == local) {
					BestAA(velocity, origin, Trace.point);
					return;
				}
			}
		}
	}*/
private:
	float GetHitDist() {
		float travel = traveledTime();
		BasePlayer* own = owner();
		typedef float(__stdcall* RetF)();
		float deltaTime = ((RetF)(g_Base + O::UnityEngine_Time::get_deltaTime))(); float time = ((RetF)(g_Base + O::UnityEngine_Time::get_time))();

		float num6 = (travel + 2 / 60.f + 0.03125f) * 1.5f;
		float maxdist = 0.1f + num6 * 5.5f;
		return maxdist;
	}
	
	Vector3 Simulate(bool returnvelocity, bool sendtoserver) {
		Vector3 pos = prevSentPosition(); Vector3 prev = prevSentVelocity(); float part = partialTime(); float travel = max(traveledTime() - sentTraveledTime(), 0.f);
		typedef Vector3(__stdcall* RetV)();
		Vector3 gr = ((RetV)(g_Base + O::UnityEngine_Physics::get_gravity))(); //static Vector3 get_gravity();


		Vector3 origin = SimulateProjectile(pos, prev, part, travel, gr * gravityModifier(), drag());

		if (sendtoserver) {
			/*if (Math::Calc3D_Dist(pos, currentPosition()) > 1.f) {
				typedef void(__stdcall* Sphere)(Vector3, float, Color, float, bool);
				((Sphere)(GetModBase() + 0x2EA8D0))(currentPosition(), 0.1f, Color(0, 0, 0, 1), 20.f, true);
				((Sphere)(GetModBase() + 0x2EA8D0))(pos, 0.1f, Color(1, 1, 1, 1), 20.f, true);
			}*/
			prevSentPosition(pos);
			prevSentVelocity(prev);
			partialTime(part);
			sentTraveledTime(traveledTime());
		}
		
		if (returnvelocity) return prev;

		return origin;
	}
};

void OnProjectileUpdate(Projectile* unk) {
	BasePlayer* owner = (BasePlayer*)safe_read(unk + 0xD0, DWORD64);

	if (Vars::Aim::Silent::Enabled && !Vars::User::Panic && (!LocalPlayer || (LocalPlayer && owner->GetSteamID() == LocalPlayer->GetSteamID()))) {
		bool ret = false;
		for (; unk->IsAlive(); unk->UpdateVelocity(0.03125f, unk, ret)) {
			if (ret) {
				break;
			}

			if (unk->launchTime() <= 0.f) {
				break;
			}
			typedef float(__stdcall* RetFEmpty)();
			float time = ((RetFEmpty)(g_Base + O::UnityEngine_Time::get_time))();

			if (time - unk->launchTime() < unk->traveledTime() + 0.03125f) {
				break;
			}
		}

		if (unk->integrity() <= 0.001f || unk->maxDistance() <= unk->traveledDistance() || unk->traveledTime() >= 8.f) {
			typedef void(__stdcall* Retire)(Projectile*);
			((Retire)(g_Base + O::Projectile::Retire))(unk);
		}
	}
	else {
		/*float travel = unk->traveledTime();
		if (travel == 0.f) {
			Projectile::CheckBullet(unk);
		}*/
		typedef void(__stdcall* Unk)(Projectile*);
		return ((Unk)retProj)(unk);
	}
}


float MaxMeleeDist(DWORD64 melee, bool localplayer) {
	float pad = 0.1f;
	typedef float(__stdcall* RetF)();
	float time = ((RetF)(g_Base + O::UnityEngine_Time::get_time))();

	float desyncTime = max(time - LocalPlayer->Time() - 0.0325f, 0.f);
	float res = pad + desyncTime * 5.5f;
	if (localplayer) {
		res += (1.5f * safe_read(melee + 0x278, float)); //maxDistance
	}
	return max(res, 0.05f);
}

DWORD64 hkMelee = 0; 
void DoMeleeAttack(AimResult target, DWORD64 active, bool transform = true) {
	if (!target.valid || !active) return;

	Vector3 lp = LocalPlayer->GetPosition();
	typedef float(__stdcall* A)();

	float time = ((A)(g_Base + O::UnityEngine_Time::get_time))();
	if (safe_read(active + O::AttackEntity::nextAttackTime, float) >= time) { //nextattacktime
		return;
	}

	if (safe_read(active + O::AttackEntity::timeSinceDeploy, float) < safe_read(active + O::AttackEntity::deployDelay, float)) {
		return;
	}

	//HitTest
	DWORD64 htstatic = safe_read(GetModBase() + S::HitTest, DWORD64);
	if (!htstatic) return;

	DWORD64 New = GetModBase() + oNew;
	typedef DWORD64(__stdcall* NNew)(DWORD64);

	DWORD64 HitTest = ((NNew)New)(htstatic);
	
	Ray ray = Ray(lp, (target.pos - lp).Normalized());
	safe_write(HitTest + O::HitTest::MaxDistance, 1000.f, float); //MaxDistance
	DWORD64 trans;
	if (transform) {
		trans = target.entity->GetTransform(Vars::Aim::AimHead ? BoneList::head : BoneList::spine3);
	}
	else {
		typedef DWORD64(__stdcall* GetTr)(DWORD64);
		trans = ((GetTr)(g_Base + O::UnityEngine_Component::get_transform))((DWORD64)target.entity);
	}
	if (!trans) return;

	safe_write(HitTest + O::HitTest::HitTransform, trans, DWORD64); //HitTransform
	safe_write(HitTest + O::HitTest::AttackRay, ray, Ray);
	//safe_write(HitTest + 0xC0, Str(xorstr(L"Flesh")), Str); //HitMaterial
	safe_write(HitTest + O::HitTest::DidHit, true, bool); //DidHit
	safe_write(HitTest + O::HitTest::HitEntity, target.entity, BasePlayer*); //HitEntity
	
	typedef Vector3(__stdcall* ITP)(DWORD64, Vector3);
	Vector3 hitpoint = ((ITP)(g_Base + O::UnityEngine_Transform::InverseTransformPoint))(trans, target.pos);
	safe_write(HitTest + O::HitTest::HitPoint, hitpoint, Vector3); //HitPoint
	safe_write(HitTest + O::HitTest::HitNormal, Vector3(0, 0, 0), Vector3); //HitNormal
	safe_write(HitTest + O::HitTest::damageProperties, safe_read(active + O::BaseMelee::damageProperties, DWORD64), DWORD64); //damageProperties

	typedef void(__stdcall* Atk)(DWORD64, DWORD64);
	
	float kd = safe_read(active + O::AttackEntity::repeatDelay, float);
	typedef void(__stdcall* StartKD)(DWORD64, float);
	((StartKD)(g_Base + O::AttackEntity::StartAttackCooldown))(active, kd);


	return ((Atk)(g_Base + O::BaseMelee::ProcessAttack))(active, HitTest);
}


void PickupItem(DWORD64 item) {
	typedef void(__stdcall* Pick)(DWORD64, Str);
	return ((Pick)(g_Base + S::ServerRPC))(item, Str(xorstr(L"Pickup")));
}

DWORD64 _hkProj = 0;
void LaunchProjectile(DWORD64 pr) {                                   //pSilent D:
	typedef void(__stdcall* Orig)(DWORD64);
	DWORD64 itemmod = safe_read(g_Base + S::ComponentGetComponentItemModProjectile, DWORD64);
	if (!itemmod || !LocalPlayer) return ((Orig)_hkProj)(pr);
	typedef DWORD64(__stdcall* GetComponent)(DWORD64, DWORD64);
	UINT64 Magazine = safe_read(pr + O::BaseProjectile::primaryMagazine, UINT64);
	DWORD64 type = safe_read(Magazine + 0x20, DWORD64);

	DWORD64 component = ((GetComponent)(g_Base + S::GetComponent))(type, itemmod);
	if (!component) return ((Orig)_hkProj)(pr);

	int numProjectiles = safe_read(component + O::ItemModProjectile::numProjectiles, int);

	typedef DWORD64(__stdcall* PoolGet)(DWORD64);
	DWORD64 _shoot = safe_read(GetModBase() + S::PoolGetProjectileShoot, DWORD64);
	DWORD64 oListProjectiles = safe_read(GetModBase() + S::List1ProjectileShootProjectile, DWORD64);
	DWORD64 test = safe_read(GetModBase() + S::BaseEntityServerRPCProjectileShoot, DWORD64);

	if (!_shoot || !oListProjectiles || !test) return ((Orig)_hkProj)((DWORD64)pr);
	DWORD64 projectileShoot = ((PoolGet)(g_Base + S::Get))(_shoot);

	DWORD64 array = ((PoolGet)(GetModBase() + oNew))(oListProjectiles);

	DWORD64 parray = il2cpp_array(CLASS(xorstr("Projectile")), numProjectiles);
	safe_write(array + 0x10, parray, DWORD64);
	//safe_write(array + );

    int ammoid = safe_read(type + 0x18, int);
	safe_write(projectileShoot + 0x18, array, DWORD64);
	safe_write(projectileShoot + 0x14, ammoid, int);

	Vector3 local = LocalPlayer->GetPosition(BoneList::neck); Vector3 velocity = Vector3(); Vector3 forward;
	
	AimResult aim = FindAimTarget(local, true, false);
	float modifier = safe_read(pr + O::BaseProjectile::projectileVelocityScale, float);

	bool isCompound = m_strcmp((char*)safe_read(safe_read(pr, DWORD64), DWORD64), xorstr("CompoundBowWeapon"));
	if (isCompound) {
		modifier += 1.f; //stringBonusVelocity
	}
	float bulletSpeed = (safe_read(component + O::ItemModProjectile::projectileVelocity, float) + safe_read(component + O::ItemModProjectile::projectileVelocitySpread, float)) * modifier * min(Vars::Weapon::BulletSpeed, 1.485f);
	if (Vars::Aim::pSilent && aim.valid) {
		//on projectile create
		forward = velocity;
	}
	else {
		typedef Vector3(__stdcall* BodyF)(DWORD64);
		forward = velocity = ((BodyF)(g_Base + O::PlayerEyes::BodyForward))(safe_read(LocalPlayer + O::BasePlayer::eyes, DWORD64));
		velocity = velocity.Normalized() * bulletSpeed;
	}

	for (int i = 0; i < numProjectiles; i++) {
		DWORD64 projectileObject = safe_read(component + O::ItemModProjectile::projectileObject, DWORD64);
		typedef pUncStr(__stdcall* Path)(DWORD64);
		pUncStr path = ((Path)(g_Base + S::get_resourcePath))(projectileObject);

		int id = safe_read(LocalPlayer + O::BasePlayer::maxProjectileID, int) + 1;
		safe_write(LocalPlayer + O::BasePlayer::maxProjectileID, id, int);

		typedef DWORD64(__stdcall* CP)(DWORD64, pUncStr, Vector3, Vector3, Vector3);
		DWORD64 projectile = ((CP)(g_Base + O::BaseProjectile::CreateProjectile))((DWORD64)pr, path, local, forward, velocity);
		if (!velocity.x && !velocity.y && !velocity.z) {
			Vector3 g = aim.pos - Vector3(0, 0.2f, 0);

			Prediction(local, g, aim.entity->GetVelocity(), bulletSpeed, ((Projectile*)projectile)->gravityModifier());
			forward = velocity = g - local;
			velocity = velocity.Normalized() * bulletSpeed;
			((Projectile*)projectile)->currentVelocity(velocity);
			((Projectile*)projectile)->initialVelocity(velocity);
		}

		if (projectile != 0) {
			safe_write(projectile + O::Projectile::mod, component, DWORD64); //mod
			safe_write(projectile + O::Projectile::seed, i, int); //seed
			safe_write(projectile + O::Projectile::owner, LocalPlayer, BasePlayer*); //owner
			safe_write(projectile + O::Projectile::projectileID, id, int); //projectileID
			safe_write(projectile + O::Projectile::invisible, false, bool); //invisible
		}
		DWORD64 projectile2 = ((PoolGet)(GetModBase() + oNew))(safe_read(GetModBase() + S::ProjectileShootProjectile, DWORD64));
		safe_write(projectile2 + 0x14, id, int); // projectileID
		safe_write(projectile2 + 0x18, local, Vector3); //startPos
		safe_write(projectile2 + 0x24, velocity, Vector3); //startVel
		safe_write(projectile2 + 0x30, i, int); //seed

		safe_write(projectile2 + 0x34, projectile, DWORD64); // TODO ебаный костыль фу (sizeof(ProjectileShoot.Projectile) = 0x30) <- сделать 2 лист/BasePlayer.createdProjectiles
		typedef void(__stdcall* Ladd)(DWORD64, DWORD64);
		((Ladd)(g_Base + S::Add))(array, projectile2);
	}
	typedef Vector3(__stdcall* Ori)(DWORD64);
	typedef void(__stdcall* RPC)(DWORD64, Str, DWORD64, DWORD64);

	
	((RPC)safe_read(test, DWORD64))((DWORD64)pr, Str(xorstr(L"CLProject")), projectileShoot, test);

	Vector3 orig = ((Ori)(g_Base + O::BasePlayer::GetInheritedProjectileVelocity))((DWORD64)LocalPlayer);

	for (int i = 0; i < numProjectiles; i++) {
		DWORD64 projectileshoot = safe_read(parray + 0x20 + (0x8 * i), DWORD64);
		Projectile* projectile = (Projectile*)safe_read(projectileshoot + 0x34, DWORD64);

		projectile->Launch(projectile);

		Vector3 hh = projectile->currentVelocity();//safe_read(projectile + 0x118, Vector3);
		projectile->currentVelocity(hh + orig);
	}
}

DWORD64 _bowProj = 0;
void DoBowAttack(DWORD64 active) {
	if (!LocalPlayer->GetKeyState(Button::FIRE_PRIMARY)) return;

	DWORD64 magazine = safe_read(active + O::BaseProjectile::primaryMagazine, DWORD64);
	int contents = safe_read(magazine + 0x1C, int); //BaseProjectile.Magazine!
	safe_write(active + O::BowWeapon::attackReady, false, bool);
	typedef void(__stdcall* TryReload)(DWORD64);
	if (contents <= 0) {
		((TryReload)(g_Base + O::BowWeapon::TryReload))(active);
		return;
	}
	
	safe_write(magazine + 0x1C, contents - 1, int);

	float kd = safe_read(active + O::AttackEntity::repeatDelay, float) + 0.1f;
	typedef void(__stdcall* StartKD)(DWORD64, float);
	((StartKD)(g_Base + O::AttackEntity::StartAttackCooldown))(active, kd);

	DWORD64 vmodel = safe_read(active + O::HeldEntity::viewModel, DWORD64);
	if (vmodel) {
		typedef void(__stdcall* Play)(DWORD64, Str);
		((Play)(g_Base + O::ViewModel::Play))(vmodel, Str(xorstr(L"attack")));
	}

	LaunchProjectile(active);

	((TryReload)(g_Base + O::BowWeapon::TryReload))(active);
	((TryReload)(g_Base + O::BaseProjectile::UpdateAmmoDisplay))(active);
	((TryReload)(g_Base + O::BaseProjectile::DidAttackClientside))(active);
}

DWORD64 _flintAttack = 0; //instant eoka
void DoFlintAttack(DWORD64 active) {
	if (!LocalPlayer->GetKeyState(Button::FIRE_PRIMARY)) return;

	DWORD64 magazine = safe_read(active + O::BaseProjectile::primaryMagazine, DWORD64);
	int contents = safe_read(magazine + 0x1C, int);
	if (contents <= 0) {
		//DryFire
		return;
	}

	safe_write(magazine + 0x1C, contents - 1, int);

	float kd = safe_read(active + O::AttackEntity::repeatDelay, float);
	typedef void(__stdcall* StartKD)(DWORD64, float);
	((StartKD)(g_Base + O::AttackEntity::StartAttackCooldown))(active, kd);

	DWORD64 vmodel = safe_read(active + O::HeldEntity::viewModel, DWORD64);
	if (vmodel) {
		typedef void(__stdcall* Play)(DWORD64, Str);
		((Play)(g_Base + O::ViewModel::Play))(vmodel, Str(xorstr(L"attack")));
	}

	LaunchProjectile(active);

	typedef void(__stdcall* TryReload)(DWORD64);
	((TryReload)(g_Base + O::BowWeapon::TryReload))(active);
	((TryReload)(g_Base + O::BaseProjectile::UpdateAmmoDisplay))(active);
	((TryReload)(g_Base + O::BaseProjectile::DidAttackClientside))(active);
} //instant eoka
