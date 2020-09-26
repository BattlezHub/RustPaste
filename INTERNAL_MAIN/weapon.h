#include "utils/xorstr.h"
float w_last_syringe = 0.f;
bool w_healing = false;

//DWORD64 oServerRPC = 0;
void WeaponPatch()
{
	if (!LocalPlayer) return;

	bool nd = true;
	BaseProjectile* weapon = LocalPlayer->GetActiveWeapon(nd);

	if (!nd) return;
	if (weapon == nullptr) return;
	//WeaponData target = weapon->Info();

	DWORD64 def = safe_read(weapon + 0x20, DWORD64);
	int category = safe_read(def + 0x40, int);

	if (Vars::Misc::FastHeal && (weapon->GetID() == 1079279582 || weapon->GetID() == -2072273936)) { //syringe/bandage
		typedef void(__stdcall* TestS)(DWORD64, Str);
		DWORD64 Held = safe_read(weapon + O::Item::heldEntity, DWORD64);
		safe_write(Held + O::MedicalTool::resetTime, 1.f, float); //disable animation

		float curtime = LocalPlayer->Time();
		bool deployed = safe_read(Held + O::HeldEntity::isDeployed, bool);
		bool pressed = LocalPlayer->GetKeyState(Button::FIRE_PRIMARY);
		//0x800 secondary
		if (pressed && deployed && curtime > w_last_syringe + 0.7f) {
			if (w_healing) {
				//printf("Used medical(kd %f) at %f(cooldown %f)\n", safe_read(Held + 0x1DC, float), curtime, (curtime - Vars::Global::last_syringe));
				TestS ss = (TestS)(g_Base + S::ServerRPC);
				ss(Held, Str(xorstr(L"UseSelf")));
			}
			else curtime += 0.3f;
			w_last_syringe = curtime;
			w_healing = true;
		}
		else if (!deployed) {
			w_healing = false;
		}
	}
	else {
		w_healing = false;
	}
	//printf("weapon id %d, category %d\n", target.id, target.category);

	if (category == 0) {
		char* classname = weapon->ClassName();

		bool weaponmelee = weapon && classname && (m_strcmp(classname, xorstr("BaseMelee")) || m_strcmp(classname, xorstr("Jackhammer"))) || m_strcmp(classname, xorstr("FlameThrower"));

		if (!weaponmelee) {
			weapon->SetRA();
			weapon->SetAutomatic();
			weapon->NoSway();

		}
		
	}

}
