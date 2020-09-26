void Misc() {
	if (!LocalPlayer) return;

	if (Vars::Misc::SpiderMan) {
		LocalPlayer->SpiderMan();
	}

}

bool wasWater = false;
DWORD64 oFixedUpdate = 0;
void ChangeSlide(DWORD64 mov, DWORD64 modelstate) {
	typedef void(__stdcall* Inp)(DWORD64, DWORD64);

	float speed = (safe_read(mov + O::PlayerWalkMovement::swimming, bool) /*swim*/ || safe_read(mov + O::BaseMovement::Duckingk__BackingField, float) /* crouch */ > 0.5f) ? 1.7f : (safe_read(mov + O::PlayerWalkMovement::jumping, bool) /*jump*/ ? 8.f : 5.5f);
	Vector3 vel = safe_read(mov + O::BaseMovement::TargetMovementk__BackingField, Vector3);
	speed = max(speed, vel.Length());
	if (!Vars::User::Panic && Vars::Misc::SlideWalk) {
		float len = vel.Length();
		if (len > 0.f) {
			vel = Vector3(vel.x / len * speed, vel.y, vel.z / len * speed);
			safe_write(mov + O::BaseMovement::TargetMovementk__BackingField, vel, Vector3);
		}
	}

	if (Vars::Misc::WalkOnWater && LocalPlayer) {
		Vector3 local = LocalPlayer->GetPosition(BoneList::l_foot);

		typedef float(__stdcall* WD)(Vector3, bool);

		float currentwater = ((WD)(GetModBase() + O::WaterLevel::GetWaterDepth))(local, true); // static float GetWaterDepth(Vector3 pos, optional bool waves);

		float upperwater = local.y - currentwater;
		upperwater -= 1.f;
		if (upperwater < 0.f && vel.y <= 0.1f) {
			vel.y = -upperwater;
		}
		safe_write(mov + O::BaseMovement::TargetMovementk__BackingField, vel, Vector3);
	}

	if (Vars::Misc::FlyHack && LocalPlayer->GetKeyState(Button::FIRE_THIRD)) { //stick to nearest player
		Vector3 local = LocalPlayer->GetPosition();
		AimResult target = FindAimTarget(local, false, false, 10.f, 0.f, true);
		if (target.valid) {
			float len = min((target.pos - local).Length(), speed);

			Vector3 direction = (target.pos - local).Normalized();
			direction = direction * len;

			safe_write(mov + O::BaseMovement::TargetMovementk__BackingField, direction, Vector3);
			vel = direction;
		}
	}
	((Inp)oFixedUpdate)(mov, modelstate); //original
}
float dlastKnock = 0.f; float dlastOpen = 0.f;

//DWORD64 oOpenDoor = 0; DWORD64 oKnockDoor = 0;
void EbynMozga(DWORD64 Door) {
	typedef void(__stdcall* Daun)(DWORD64, BasePlayer*);
	float time = LocalPlayer->Time();
	if (time > dlastKnock + 0.5f) {
		((Daun)(g_Base + O::Door::Menu_KnockDoor))(Door, LocalPlayer);
		dlastKnock = time;
	}

	if (time > dlastOpen + 0.1f) {
		((Daun)(g_Base + O::Door::Menu_OpenDoor))(Door, LocalPlayer);
		dlastOpen = time;
	}

}
/*void EnableDebugCamera() {
	DWORD64 array = il2cpp_value(InitClass(xorstr("ConsoleGen")), xorstr("All"));
	int size = safe_read(array + 0x18, int);

	int found = 0;
	for (int i = 0; i < size; i++) {
		DWORD64 command = safe_read(array + 0x20 + 0x8 * i, DWORD64);
		pUncStr fullname = (pUncStr)safe_read(command + 0x20, DWORD64);

		if (!m_wcsicmp(fullname->str, xorstr(L"chat.say")) ) {
			found++;
			safe_write(command + 0x57, false, bool); //AllowRunFromServer
		}
	}
} */

//DWORD64 oGetTr = 0; DWORD64 oTrSetP = 0; DWORD64 oSendTick = 0; DWORD64 oTeleport = 0; DWORD64 oTimeSinceStartup = 0;
/*void BestAA(Vector3 inVelocity, Vector3 prev, Vector3 cur) {
	if (!LocalPlayer) return;

	typedef void(__stdcall* Line)(Vector3, Vector3, Color, float, bool, bool);
	((Line)(GetModBase() + 0x2E9820))(prev, cur, Color(1, 0, 0, 1), 20.f, true, true);

	Vector3 AAVel = Vector3(random() % 100, random() % 100, -random() % 100).Normalized();
	typedef float(__stdcall* RetF)();
	float time = ((RetF)oTimeSinceStartup)();
	float deltaTime = time - LocalPlayer->Time();

	DWORD64 mov = safe_read(LocalPlayer + oPlayerMovement, DWORD64);
	float speed = (safe_read(mov + 0x12A, bool)  || safe_read(mov + 0x44, float)  > 0.5f) ? 1.7f : (safe_read(mov + 0x12C, bool) ? 8.f : 5.5f);
	float maxDist = min(deltaTime * speed, 5.f);

	Vector3 position = LocalPlayer->GetPosition(BoneList::pelvis);
	((Line)(GetModBase() + 0x2E9820))(cur, position, Color(1, 0.92, 0.016, 1), 20.f, true, true);


	position += AAVel * maxDist;
	typedef void(__stdcall* Teleport)(DWORD64, Vector3, BasePlayer*);

	((Line)(GetModBase() + 0x2E9820))(cur, position, Color(0, 1, 0, 1), 20.f, true, true);

	((Teleport)oTeleport)(mov, position, LocalPlayer);

	typedef void(__stdcall* Tick)(BasePlayer*);
	((Tick)oSendTick)(LocalPlayer);
}*/
