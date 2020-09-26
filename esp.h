void DrawCrosshair() {
	if (!Vars::Visuals::Crosshair) return;
	const float size = Vars::Visuals::crosshairSize;

	GUI::VerticalLine(Vector2(Vars::User::ScreenWidth / 2, Vars::User::ScreenHeight / 2 - size), size * 2, Color(1, 0.33, 0, 1)); //orange
	GUI::HorizontalLine(Vector2(Vars::User::ScreenWidth / 2 - size, Vars::User::ScreenHeight / 2 ), size * 2, Color(1, 0.33, 0, 1)); //orange
}

bool DrawPlayer(BasePlayer* ply) {
	if (Vars::PlayerEsp::IgnoreSleepers && ply->HasFlags(16)) return false;

	const Color BoxVisibleColor = Color(0.16, 0.55, 0.25, 1); //green
	const Color BoxInVisibleColor = Color(1, 0, 0.08, 1); //red
	const Color BoxTeammateColor = Color(0, 0.23, 1, 1); //blue
	const Color BoxFillColor = Color(0.33, 0.33, 0.33, 0.5); //gray

	Vector3 pos = ply->GetPosition(BoneList(jaw)) + Vector3(0.f, 0.16f, 0.f); //bit up
	Vector2 FeetR, FeetL, Head;
	if (!LocalPlayer->WorldToScreen(pos, Head) || !LocalPlayer->WorldToScreen(ply->GetPosition(BoneList(r_foot)), FeetR)
		|| !LocalPlayer->WorldToScreen(ply->GetPosition(BoneList(l_foot)), FeetL)) {
		return false;
	}
	Vector2 lowestFoot = (FeetR.y < FeetL.y ? FeetR : FeetL);
	float ply_h = -(Head.y - lowestFoot.y);
	if (ply_h < 0.f) ply_h = -ply_h;
	float ply_x = lowestFoot.x - ply_h / 4;
	float ply_y = lowestFoot.y;
	float ply_w = ply_h / 2;
	
	//box
	Rect box = Rect(lowestFoot.x, lowestFoot.y, ply_w, -ply_h);
	if (Vars::PlayerEsp::Box) {
		bool teammate = ply->IsTeamMate();
		bool vis;
		if (!teammate) {
			vis = ply->IsVisible();
		}
		Color clr = !teammate ? (vis ? BoxVisibleColor : BoxInVisibleColor) : BoxTeammateColor;

		GUI::Box(Vector2(Head.x - ply_w / 2, Head.y), Vector2(ply_w, ply_h), clr, Vars::PlayerEsp::BoxThick);

		if (Vars::PlayerEsp::FillBox) {
			GUI::FillBox(Rect(Head.x - ply_w / 2, Head.y, ply_w, ply_h), BoxFillColor);
		}
	}

	

	int dist = ((int)(Math::Calc3D_Dist(LocalPlayer->GetPosition(), pos) * 10)) / 10;
	float tsize = (2000.f - dist) / 150;

	float Cury = 0.f;
	if (Vars::PlayerEsp::DrawHpBar) {
		GUI::FillBox(Rect(Head.x - ply_w / 2 - 2.f, ply_y, min(ply_w * 0.3f, 3.5f), -ply_h * (ply->GetHealth() / 100)), Color(0, 1, 0, 1));
		GUI::Box(Vector2(Head.x - ply_w / 2 - 2.f, ply_y), Vector2(min(ply_w * 0.3f, 3.5f), -ply_h), Color(0, 0, 0, 1), min(ply_w / 10, 1.f));
	}
	if (Vars::PlayerEsp::DrawNick) {
		const wchar_t* name = ply->GetName();
		if (name != nullptr) {
			GUI::Label(Rect(Head.x + ply_w * 1.05f, Head.y + Cury, 150, 20), Str(name), Color(0, 0.9, 1, 1), false);
			Cury += tsize;
		}
		
	}
	if (Vars::PlayerEsp::DrawWeapon) {
		bool _d = false;
		const wchar_t* name = ply->GetActiveWeapon(_d)->GetName();
		if (name != nullptr) {
			GUI::Label(Rect(Head.x + ply_w * 1.05f, Head.y + Cury, 200, 20), Str(name), Color(0, 0.9, 1, 1), false);
		}
	}
	return true;
}
DWORD64 ClientEntities = 0;

void PickupItem(DWORD64 item);
void DoMeleeAttack(AimResult target, DWORD64 active, bool transform);
void ESPLoop() {
	if (ClientEntities == 0) {
		ClientEntities = il2cpp_value(InitClass(xorstr("BaseNetworkable")), xorstr("clientEntities"));
		if (!ClientEntities) return;
	}

	UINT64 ClientEntities_list = safe_read(ClientEntities + 0x10, UINT64);
	UINT64 ClientEntities_values = safe_read(ClientEntities_list + 0x28, UINT64);
	if (!ClientEntities_values) return;
	int EntityCount = safe_read(ClientEntities_values + 0x10, int);
	//printf("bn size %d\n", EntityCount);
	UINT64 EntityBuffer = safe_read(ClientEntities_values + 0x18, UINT64);

	bool _i = false;
	BaseProjectile* weapon = LocalPlayer->GetActiveWeapon(_i);
	DWORD64 active = safe_read(weapon + O::Item::heldEntity, DWORD64);
	char* classname = weapon->ClassName();

	bool weaponmelee = weapon && classname && (m_strcmp(classname, xorstr("BaseMelee")) || m_strcmp(classname, xorstr("Jackhammer")));
	for (int i = 0; i < EntityCount; i++) {
		UINT64 Entity = safe_read(EntityBuffer + 0x20 + (i * 0x8), UINT64); if (Entity <= 100000) continue;
		UINT64 Object = safe_read(Entity + 0x10, UINT64); if (Object <= 100000) continue;
		UINT64 ObjectClass = safe_read(Object + 0x30, UINT64); if (ObjectClass <= 100000) continue;
		pUncStr name = safe_read(ObjectClass + 0x60, pUncStr); if (!name) continue;
		char* buff = name->stub;
		DWORD64 ent = safe_read(Object + 0x28, UINT64);
		if ((Vars::PlayerEsp::Enabled || (Vars::Aim::MeleeAimPlayers && weaponmelee)) && m_strstr(buff, xorstr("player.prefab"))) {
			if(Vars::PlayerEsp::Enabled) DrawPlayer((BasePlayer*)ent);

			if (Vars::Aim::MeleeAimPlayers && weaponmelee) {
				
				AimResult target = TargetMeleeTest((BasePlayer*)ent, active);
				DoMeleeAttack(target, active, true);
			}
		}
		else if (Vars::Misc::DoorKnocker && m_strstr((char*)safe_read(safe_read(ent, DWORD64) + 0x10, DWORD64), xorstr("Door"))) {
			UINT64 gameObject = safe_read(ObjectClass + 0x30, UINT64); 
			UINT64 Trans = safe_read(gameObject + 0x8, UINT64);
			UINT64 Vec = safe_read(Trans + 0x38, UINT64);
			Vector3 pos = safe_read(Vec + 0x90, Vector3);
			typedef Vector3(__stdcall* CPoint)(BasePlayer*, Vector3);

			Vector3 local = ((CPoint)(g_Base + O::BaseEntity::ClosestPoint))(LocalPlayer, pos);
			if (ent && Math::Calc3D_Dist(local, pos) < 3.f) {
				EbynMozga(ent);
			}
		}
		else if (Vars::Aim::MeleePickup && m_strstr(buff, xorstr("/collectable/"))) {
			UINT64 gameObject = safe_read(ObjectClass + 0x30, UINT64);
			UINT64 Trans = safe_read(gameObject + 0x8, UINT64);
			UINT64 Vec = safe_read(Trans + 0x38, UINT64);
			Vector3 pos = safe_read(Vec + 0x90, Vector3);

			typedef Vector3(__stdcall* CPoint)(BasePlayer*, Vector3);

			Vector3 local = ((CPoint)(g_Base + O::BaseEntity::ClosestPoint))(LocalPlayer, pos);
			if (Math::Calc3D_Dist(local, pos) < 3.f) {
				PickupItem(ent);
			}
		}
		else if (Vars::Aim::MeleeAutoFarm && weaponmelee && (m_strstr((char*)safe_read(safe_read(ent, DWORD64) + 0x10, DWORD64), xorstr("TreeEntity")) || m_strstr((char*)safe_read(safe_read(ent, DWORD64) + 0x10, DWORD64), xorstr("OreHotSpot")))) {
			char* name = (char*)safe_read(safe_read(ent, DWORD64) + 0x10, DWORD64);
			UINT64 gameObject = safe_read(ObjectClass + 0x30, UINT64);
			UINT64 Trans = safe_read(gameObject + 0x8, UINT64);
			UINT64 Vec = safe_read(Trans + 0x38, UINT64);
			Vector3 pos = safe_read(Vec + 0x90, Vector3); //TODO Hit tree marker
			typedef Vector3(__stdcall* CPoint)(BasePlayer*, Vector3);

			Vector3 local = ((CPoint)(g_Base + O::BaseEntity::ClosestPoint))(LocalPlayer, pos);
			if (Math::Calc3D_Dist(local, pos) >= 3.f) {
				continue;
			}

			AimResult target = AimResult();
			target.valid = true;
			target.pos = pos;
			target.entity = (BasePlayer*)ent;
			DoMeleeAttack(target, active, false);
		}

		
	}
}