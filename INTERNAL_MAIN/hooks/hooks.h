
#include <ddraw.h>
#include "../utils/memory.h"
#include "../gui/gui.h"
#include "../utils/il2cpp.h"
#include "../utils/xorstr.h"
bool fly_just_disabled = false;
namespace Hooks {
	bool wasFLagging = false;
	void ClientInput(DWORD64 Lp, DWORD64 ModelState) {
		if (!LocalPlayer) {
			FindAimTarget(Vector3(0, 0, 0), true, false);
		}
		if (!wasFLagging && Vars::Misc::FakeLag) {
			safe_write(LocalPlayer + O::BasePlayer::clientTickInterval, 0.4f, float);
			wasFLagging = true;
		}
		else if (wasFLagging && !Vars::Misc::FakeLag) {
			safe_write(LocalPlayer + O::BasePlayer::clientTickInterval, 0.05f, float);
			wasFLagging = false;
		}


		DWORD64 Mov = safe_read(Lp + O::BasePlayer::movement, DWORD64);
		if (Vars::Misc::FlyHack) {
			safe_write(Mov + O::BaseMovement::adminCheat, true, bool);
			fly_just_disabled = true;
		}
		else if (fly_just_disabled) {
			safe_write(Mov + O::BaseMovement::adminCheat, false, bool);
			fly_just_disabled = false;
		}

		float time = LocalPlayer->Time();
		if (time > g_GUICalled + 10.f) {
			DWORD64 DDraw = g_Base + O::UnityEngine_DDraw::Get;
			typedef void(__stdcall* TD)();
			((TD)DDraw)();
			g_GUICalled = time;
		}

		if (!mfound) {
			FindMatrix();
		}
		WeaponPatch();
		Misc();


		//virtual void ClientInput(InputState state);
		DWORD64 Original = GetModBase() + 0x289970;
		typedef void(__stdcall* Orig)(DWORD64, DWORD64);
		((Orig)Original)(Lp, ModelState);

		if (Vars::Misc::SlideWalk) {
			LocalPlayer->SetModelFlag(0x10); //sprinting
		}

		if (Vars::Misc::SwimOnGround) {
			LocalPlayer->SetWaterLevel(0.65f);
		}

		if (Vars::Misc::FlyHack) {
			DWORD64 mstate = safe_read(Lp + O::BasePlayer::modelState, DWORD64);
			int flags = safe_read(mstate + O::ModelState::flags, int);
			flags &= ~0x40; //flying
			safe_write(mstate + O::ModelState::flags, flags, int);
		}


	}

	void HookVTable(const char* classname, DWORD64 search, DWORD64 target, PDWORD64 orig) {
		DWORD64 table = InitClass(classname);
		for (DWORD64 i = table; i <= table + 0x1500; i += 0x1) {
			DWORD64 addr = safe_read(i, DWORD64);
			if (addr == search) {
				safe_write(i, target, DWORD64);
				*orig = addr;
				return;
			}
		}
	}

	/*DWORD64 _pNetwork = 0;
	void OnNetworkMessage(DWORD64 message) {
		int type = safe_read(message + 0x10, int);
		if (type != 0x6) {
			typedef void(__stdcall* NetworkMessage)(DWORD64);
			return ((NetworkMessage)_pNetwork)(message);
		}
	}*/
	bool Init() {
		/*DWORD64 Client = InitClass(xorstr("Client"));
		DWORD64 NM = safe_read(Client + 0x40, DWORD64); //unk
		il2cpp_hook(il2cpp_method(CLASS(xorstr("Client")), xorstr("OnNetworkMessage")), &OnNetworkMessage, &_pNetwork);

		il2cpp_hook(NM + 0x1B0, &OnNetworkMessage, &_pNetwork);*/

		DWORD64 kl = CLASS(xorstr("DDraw"), xorstr("UnityEngine"));
		DWORD64 mt = il2cpp_method(kl, xorstr("OnGUI"));
		il2cpp_hook(mt, &MainTick, &retMainTick);

		HookVTable(xorstr("FlintStrikeWeapon"), METHOD(CLASS(xorstr("FlintStrikeWeapon")), xorstr("DoAttack")), (DWORD64)&DoFlintAttack, &_bowProj); //0xAAA
		HookVTable(xorstr("BowWeapon"), METHOD(CLASS(xorstr("BowWeapon")), xorstr("DoAttack")), (DWORD64)&DoBowAttack, &_bowProj); //0xAA0
		HookVTable(xorstr("BaseProjectile"), METHOD(CLASS(xorstr("BaseProjectile")), xorstr("LaunchProjectile")), (DWORD64)&LaunchProjectile, &_hkProj);

		il2cpp_hook(il2cpp_method(CLASS(xorstr("Projectile")), xorstr("Update")), &OnProjectileUpdate, &retProj);
		il2cpp_hook(il2cpp_method(CLASS(xorstr("PlayerWalkMovement")), xorstr("DoFixedUpdate")), &ChangeSlide, &oFixedUpdate);
		HookVTable(xorstr("PlayerWalkMovement"), METHOD(CLASS(xorstr("PlayerWalkMovement")), xorstr("DoFixedUpdate")), (DWORD64)&ChangeSlide, &oFixedUpdate);

		DWORD64 BPlayer = safe_read(g_Base + S::BasePlayer, DWORD64); //ScanFor(BasePlayer)
		safe_write(BPlayer + vClientInputIndex, (DWORD64)&ClientInput, DWORD64);
		return true;
	}

}