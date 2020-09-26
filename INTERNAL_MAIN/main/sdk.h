enum BoneList : int
{
	pelvis = 0,
	l_hip = 1,
	l_knee = 2,
	l_foot = 3,
	l_toe = 4,
	l_ankle_scale = 5,
	penis = 6,
	GenitalCensor = 7,
	GenitalCensor_LOD0 = 8,
	Inner_LOD0 = 9,
	GenitalCensor_LOD1 = 11,
	GenitalCensor_LOD2 = 12,
	r_hip = 13,
	r_knee = 14,
	r_foot = 15,
	r_toe = 16,
	r_ankle_scale = 17,
	spine1 = 18,
	spine1_scale = 19,
	spine2 = 20,
	spine3 = 21,
	spine4 = 22,
	l_clavicle = 23,
	l_upperarm = 24,
	l_forearm = 25,
	l_hand = 26,
	l_index1 = 27,
	l_index2 = 28,
	l_index3 = 29,
	l_little1 = 30,
	l_little2 = 31,
	l_little3 = 32,
	l_middle1 = 33,
	l_middle2 = 34,
	l_middle3 = 35,
	l_prop = 36,
	l_ring1 = 37,
	l_ring2 = 38,
	l_ring3 = 39,
	l_thumb1 = 40,
	l_thumb2 = 41,
	l_thumb3 = 42,
	IKtarget_righthand_min = 43,
	IKtarget_righthand_max = 44,
	l_ulna = 45,
	neck = 46,
	head = 47,
	jaw = 48,
	eyeTranform = 49,
	l_eye = 50,
	l_Eyelid = 51,
	r_eye = 52,
	r_Eyelid = 53,
	r_clavicle = 54,
	r_upperarm = 55,
	r_forearm = 56,
	r_hand = 57,
	r_index1 = 58,
	r_index2 = 59,
	r_index3 = 60,
	r_little1 = 61,
	r_little2 = 62,
	r_little3 = 63,
	r_middle2 = 65,
	r_middle3 = 66,
	r_prop = 67,
	r_ring1 = 68,
	r_ring2 = 69,
	r_ring3 = 70,
	r_thumb1 = 71,
	r_thumb2 = 72,
	r_thumb3 = 73,
	IKtarget_lefthand_min = 74,
	IKtarget_lefthand_max = 75,
	r_ulna = 76,
	l_breast = 77,
	r_breast = 78,
	BoobCensor = 79,
	BreastCensor_LOD0 = 80,
	BreastCensor_LOD1 = 83,
	BreastCensor_LOD2 = 84
};

bool PLOS(Vector3 a, Vector3 b);
class BaseProjectile {
public:
	char* ClassName() {
		return (char*)safe_read(safe_read(safe_read(this + O::Item::heldEntity, DWORD64), DWORD64) + 0x10, DWORD64);
	}
	const wchar_t* GetName() {
		if (this == nullptr) return nullptr;
		DWORD64 held = safe_read(this + O::Item::heldEntity, DWORD64);
		if (!held) return nullptr;

		typedef pUncStr(__stdcall* GetName)(DWORD64);
		pUncStr nm = ((GetName)(g_Base + O::BaseNetworkable::get_ShortPrefabName))(held);
		if (!nm) return nullptr;
		return nm->str;
	}
	int GetUID() {
		return safe_read(this + O::Item::uid, int);
	}
	int LoadedAmmo() {
		UINT64 Held = safe_read((UINT64)this + O::Item::heldEntity, UINT64);
		if (Held <= 0) return 0;
		UINT64 Magazine = safe_read(Held + O::BaseProjectile::primaryMagazine, UINT64);
		if (Magazine <= 0 || Magazine == 0x3F000000) {
			return 0;
		}
		UINT64 ammoType = safe_read(Magazine + 0x20, UINT64); //BaseProjectile.Magazine!
		if (ammoType <= 0 || ammoType == 0x3F000000) {
			return 0;
		}
		int ammo = safe_read(ammoType + O::ItemDefinition::itemid, int);
		return ammo;
	}
	int LoadedCount() {
		UINT64 Held = safe_read((UINT64)this + O::Item::heldEntity, UINT64);
		if (Held <= 0) return 0;
		UINT64 Magazine = safe_read(Held + O::BaseProjectile::primaryMagazine, UINT64);
		if (Magazine <= 0 || Magazine == 0x3F000000) {
			return 0;
		}
		int ammo = safe_read(Magazine + 0x1C, int); //BaseProjectile.Magazine!
		return ammo;
	}
	int GetID() {
		UINT64 Info = safe_read((UINT64)this + O::Item::info, UINT64);
		int ID = safe_read(Info + O::ItemDefinition::itemid, int);
		return ID;
	}


	void SetRA() {
		if (!Vars::Weapon::NoRecoil) return;
		UINT64 Held = safe_read(this + O::Item::heldEntity, UINT64);
		UINT64 RecoilProperties = safe_read(Held + O::BaseProjectile::recoil, UINT64);

		safe_write(RecoilProperties + O::RecoilProperties::recoilYawMin, 0.f, float);
		safe_write(RecoilProperties + O::RecoilProperties::recoilYawMax, 0.f, float);
		safe_write(RecoilProperties + O::RecoilProperties::recoilPitchMin, 0.f, float);
		safe_write(RecoilProperties + O::RecoilProperties::recoilPitchMax, 0.f, float);


	}
	void SetAutomatic()
	{
		if (Vars::Weapon::Automatic)
		{
			ULONG64 Held = safe_read(this + O::Item::heldEntity, ULONG64);
			safe_write(Held + O::BaseProjectile::automatic, 1, bool);
		}
	}
	void NoSway()
	{
		if (Vars::Weapon::NoSway)
		{
			ULONG64 Held = safe_read(this + O::Item::heldEntity, ULONG64);
			safe_write(Held + O::BaseProjectile::aimSway, 0, float); //aimSway
			safe_write(Held + O::BaseProjectile::aimSwaySpeed, 0, float); //aimSwaySpeed
		}
	}
};

class BasePlayer;

Matrix4x4* pViewMatrix = nullptr;
BasePlayer* LocalPlayer = 0;

//DWORD64 oTime = 0;
class BasePlayer
{
public:
	bool WorldToScreen(const Vector3& EntityPos, Vector2& ScreenPos)
	{
		if (!pViewMatrix) return false;
		Vector3 TransVec = Vector3(pViewMatrix->_14, pViewMatrix->_24, pViewMatrix->_34);
		Vector3 RightVec = Vector3(pViewMatrix->_11, pViewMatrix->_21, pViewMatrix->_31);
		Vector3 UpVec = Vector3(pViewMatrix->_12, pViewMatrix->_22, pViewMatrix->_32);
		float w = Math::Dot(TransVec, EntityPos) + pViewMatrix->_44;
		if (w < 0.098f) return false;
		float y = Math::Dot(UpVec, EntityPos) + pViewMatrix->_42;
		float x = Math::Dot(RightVec, EntityPos) + pViewMatrix->_41;
		Vector2 res = Vector2((Vars::User::ScreenWidth / 2) * (1.f + x / w), (Vars::User::ScreenHeight / 2) * (1.f - y / w));
		if (res.x > Vars::User::ScreenWidth || res.y > Vars::User::ScreenHeight) {
			return false;
		}
		ScreenPos = res;
		return true;
	}

	bool GetKeyState(Button b) {
		DWORD64 InputState = safe_read(safe_read(this + O::BasePlayer::input, DWORD64) + O::PlayerInput::state, DWORD64);
		DWORD64 Cur = safe_read(InputState + O::InputState::current, DWORD64);
		if (!Cur) return false;
		int btn = safe_read(Cur + O::InputMessage::buttons, int);
		return ((btn & (int)b) == (int)b);
	}

	wchar_t* GetName() {
		pUncStr name = (pUncStr)safe_read(this + O::BasePlayer::_displayName, DWORD64);
		return name->str;
	}
	bool IsValid() {
		bool res = true;
		if (safe_read(this + O::BaseNetworkable::IsDestroyedk__BackingField, bool) || !safe_read(this + O::BaseNetworkable::net, DWORD64)) res = false;
		if (res) {
			if (IsDead()) res = false;
		}
		return res;
	}

	//*** base entity info ***//
	float GetHealth() {
		return safe_read(this + O::BaseCombatEntity::_health, float);
	}

	Vector3 GetVelocity() {
		ULONG64 PlayerModel = safe_read(this + O::BasePlayer::playerModel, ULONG64);
		return safe_read(PlayerModel + O::PlayerModel::newVelocity, Vector3);
	}
	void SetFlag(int Flg) {
		int flag = safe_read(this + O::BasePlayer::playerFlags, int);
		safe_write(this + O::BasePlayer::playerFlags, flag |= Flg, int);
	}

	bool HasFlags(int Flg) {
		return (safe_read(this + O::BasePlayer::playerFlags, int) & Flg);
	}

	bool IsSleeping() { return HasFlags(16); }
	bool IsWounded() { return HasFlags(64); }
	bool IsTeamMate()
	{
		ULONG64 ClientTeam = safe_read(LocalPlayer + O::BasePlayer::clientTeam, ULONG64);
		ULONG64 members = safe_read(ClientTeam + O::ProtoBuf_PlayerTeam::members, ULONG64);
		int size = safe_read(members + 0x18, int);
		ULONG64 List = safe_read(members + 0x10, ULONG64);

		DWORD64 Search = this->GetSteamID();
		for (int i = 0; i < size; i++) {
			ULONG64 player = safe_read(List + 0x20 + (i * 0x8), ULONG64);
			DWORD64 id = safe_read(player + 0x20, ULONG64);
			if (id == Search) return true;
		}
		return false;
	}

	bool IsDead() {
		if (!this) return true;
		return safe_read(this + O::BaseCombatEntity::lifestate, bool);
	}

	ULONG64 GetSteamID() {
		return safe_read(this + O::BasePlayer::userID, ULONG64);
	}

	bool IsVisible() {
		typedef bool(__stdcall* LOS)(Vector3, Vector3, int, float);
		bool canIgnore = PLOS(LocalPlayer->GetPosition(), this->GetPosition());
		return canIgnore;
	}
	//*** localplayer ***//
	bool IsMenu() {
		if (!this) return true;
		ULONG64 Input = safe_read(this + O::BasePlayer::input, ULONG64);
		return !(safe_read(Input + O::PlayerInput::hasKeyFocus, bool));
	}

	BaseProjectile* GetActiveWeapon(bool& isnew) {
		int ActUID = safe_read(this + O::BasePlayer::clActiveItem, int);
		if (!ActUID) return nullptr;

		Vars::User::last_active_id = ActUID;
		BaseProjectile* ActiveWeapon;

		ULONG64 Inventory = safe_read(this + O::BasePlayer::inventory, ULONG64);
		ULONG64 Belt = safe_read(Inventory + O::PlayerInventory::containerBelt, ULONG64);
		ULONG64 ItemList = safe_read(Belt + O::ItemContainer::itemList, ULONG64);
		ULONG64 Items = safe_read(ItemList + 0x10, ULONG64);
		for (int i = 0; i < 6; i++)
			if (ActUID == (ActiveWeapon = (BaseProjectile*)safe_read(Items + 0x20 + (i * 0x8), ULONG64))->GetUID()) {
				return ActiveWeapon;
			}

		return 0;
	}

	void SetWaterLevel(float val) {
		DWORD64 mstate = safe_read(this + O::BasePlayer::modelState, DWORD64);
		safe_write(mstate + O::ModelState::waterLevel, val, float);
	}
	void SetModelFlag(int flag) {
		DWORD64 mstate = safe_read(this + O::BasePlayer::modelState, DWORD64);

		int flags = safe_read(mstate + O::ModelState::flags, int);
		safe_write(mstate + O::ModelState::flags, flags |= flag, int);
	}
	//*** cheat func ***//

	void SpiderMan() {
		ULONG64 Movement = safe_read(this + O::BasePlayer::movement, ULONG64);
		safe_write(Movement + O::PlayerWalkMovement::groundAngleNew, 0.f, float);
	}


	Vector3 GetPosition(int bone = 46) {
		return GetBone(GetTransform(bone));
	}

	typedef Vector3(__stdcall* Transform)(UINT64);
	Vector3 GetBone(ULONG_PTR pTransform)
	{
		if (!pTransform) return Vector3();
		Transform original = (Transform)(g_Base + O::UnityEngine_Transform::get_position);
		Vector3 res = original(pTransform);
		//printf("[+] %llX -> %f %f %f\n", pTransform, res.x, res.y, res.z);
		return res;
	}

	float Time() {
		return safe_read(this + O::BasePlayer::lastSentTickTime, float);
	}
	ULONG64 GetTransform(int bone)
	{
		DWORD64 entity_model = safe_read(this + O::BaseEntity::model, DWORD64);
		DWORD64 bone_dict = safe_read(entity_model + O::Model::boneTransforms, DWORD64);
		DWORD64 BoneValue = safe_read(bone_dict + 0x20 + bone * 0x8, DWORD64);

		return BoneValue;
	}
};


