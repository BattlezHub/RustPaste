class AimResult {
public:
	bool valid;
	BasePlayer* entity;
	Vector3 pos;
	float dist = 10000.f;
	float realdist = 0.f;
	float fov = 10000.f;

	bool operator<(const AimResult& b) {
		if (fov == 10000.f) {
			return this->dist < b.dist;
		}
		else{
			return this->fov < b.fov;
		}
	}

	AimResult() {
		this->valid = false;
		this->fov = 10000.f;
	}
	AimResult(Vector3 target) {
		this->valid = false;
		this->pos = target;
	}
};

DWORD64 oPlayerList = 0;
float GetFov(Vector3 Pos);
bool PLOS(Vector3 a, Vector3 b);
AimResult FindAimTarget(Vector3 from, bool sortByFov, bool silent, float maxdist = Vars::Aim::MaxDist, float silent_traveldist = 0.f, bool visible = false) {
	AimResult lowest = AimResult();

	if (!oPlayerList) {
		DWORD64 val = safe_read(g_Base + S::BasePlayer, DWORD64);
		DWORD64 st = safe_read(val + 0xB8, DWORD64);
		oPlayerList = safe_read(st + 0x8, DWORD64);
	}
	UINT64 ClientEntities_values = safe_read(oPlayerList + 0x28, UINT64);
	if (!ClientEntities_values) return lowest;
	int EntityCount = safe_read(ClientEntities_values + 0x10, int);
	//printf("bn size %d\n", EntityCount);
	UINT64 EntityBuffer = safe_read(ClientEntities_values + 0x18, UINT64);
	for (int i = 0; i <= EntityCount; i++)
	{
		BasePlayer* Player = (BasePlayer*)safe_read(EntityBuffer + 0x20 + (i * 0x8), UINT64);

		if (!Player->IsValid()) continue;

		if (!LocalPlayer || Player->GetSteamID() == LocalPlayer->GetSteamID()) {
			LocalPlayer = Player; continue;
		}

		if (!Vars::Aim::AimOnSleepers && Player->IsSleeping()) continue;
		if (!Vars::Aim::AimOnWounded && Player->IsWounded()) continue;
		if (!Vars::Aim::AimOnTeam && Player->IsTeamMate()) continue;

		Vector3 pos = Player->GetPosition();
		
		typedef bool(__stdcall* LOS)(Vector3, Vector3, int, float);
		AimResult res = AimResult();

		res.valid = true;
		if (visible) {
			res.valid = PLOS(from, pos);
			if (!res.valid) continue;
		}

		float dst = Math::Calc3D_Dist(from, pos);
		if (silent) {
			res.realdist = dst;

			dst += silent_traveldist;
			if (!Vars::Aim::Silent::Shoot300 && dst > 300.f) continue;
			if (!Vars::Aim::Silent::Shoot150 && dst > 150.f) continue;
			if (!Vars::Aim::Silent::Shoot80 && dst > 80.f) continue;
		}
		else {
			if (dst > maxdist) continue;
		}

		if (!sortByFov) {
			res.dist = dst;
		}
		else {
			res.fov = GetFov(pos);
			if (res.fov >= Vars::Aim::Fov) continue;
		}
		res.entity = Player;
		res.pos = pos;
		if (res < lowest) lowest = res;
	}
	return lowest;
}


AimResult FindProjectileTarget(Vector3 from, float traveldist) {
	return FindAimTarget(from, false, true, Vars::Aim::MaxDist, traveldist);
}

//DWORD64 oClosestPoint = 0;
float MaxMeleeDist(DWORD64 melee, bool localplayer);
AimResult TargetMeleeTest(BasePlayer* Player, DWORD64 melee) {
	AimResult res = AimResult();
	if (!Player || !Player->IsValid()) return res;
	
	if (Player->IsWounded()) return res;
	if (!Vars::Aim::AimOnSleepers && Player->IsSleeping()) return res;
	if (!Vars::Aim::AimOnTeam && Player->IsTeamMate()) return res;
	typedef Vector3(__stdcall* CPoint)(BasePlayer*, Vector3);

	Vector3 prepos = Player->GetPosition(BoneList::head/*we dont care about bone*/);

	Vector3 closest_entity = ((CPoint)(g_Base + O::BaseEntity::ClosestPoint))(LocalPlayer, prepos);
	Vector3 closest_local = ((CPoint)(g_Base + O::BaseEntity::ClosestPoint))(Player, closest_entity);
	float disttoentity = MaxMeleeDist(melee, false);
	float distfromlocal = MaxMeleeDist(melee, true);

	float maxdist = distfromlocal + disttoentity;

	float realdist = (closest_local - closest_entity).Length();

	if (realdist > maxdist) return res;
	
	Vector3 target;
	if (realdist <= disttoentity) {
		target = closest_entity;
	}
	else {
		Vector3 dir = (closest_entity - closest_local).Normalized();
		target = closest_local + dir * disttoentity;
	}
	res.valid = PLOS(closest_local, target);
	if (!res.valid) return res;
	
	Vector3 lppos = LocalPlayer->GetPosition();
	res.valid = PLOS(lppos, target);
	if (!res.valid) return res;

	res.dist = realdist;
	res.entity = Player;
	res.pos = target;
	return res;
}

