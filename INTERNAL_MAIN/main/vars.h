class BaseProjectile;
namespace Vars {
	namespace User {
		float ScreenWidth = 1920;
		float ScreenHeight = 1080;

		bool Panic = false;

		bool MenuOpen = true;
		bool InsertPressed = false;

		int activetab = 0;

		int last_active_id = 0;

		float lastSentTickTime = 0.f;
	}
	namespace Visuals {
		bool Crosshair = true;
		float crosshairSize = 5.f;
	}
	namespace PlayerEsp {
		bool Enabled = true;
		bool IgnoreSleepers = true;

		bool Logo = true;
		bool Box = true;
		bool FillBox = true;
		
		float BoxThick = 1.5f;

		bool DrawNick = true;
		bool DrawHpBar = true;
		bool DrawWeapon = true;
	}

	namespace Aim {
		namespace Silent {
			bool Enabled = false;
			float MaxBulletTravelDistance = 150.f;
			//костыль
			bool Shoot80 = false;
			bool Shoot150 = false;
			bool Shoot300 = false;
		}
		bool pSilent = true;

		bool FakeDist = false;

		bool AimOnSleepers = true;
		bool AimOnWounded = true;
		bool AimOnTeam = false;

		float MaxDist = 500.f;
		float Fov = 80.f;
		
		bool AimHead = false;
		bool AimBody = true;

		bool MeleeAimPlayers = false;
		bool MeleeAutoFarm = false;
		bool MeleePickup = false;
	}

	namespace Weapon {
		float BulletSpeed = 1.5f;
		bool FatBullet = true;

		bool Automatic = true;
		bool NoRecoil = false;
		bool SuperEoka = true;
		bool NoSway = true;
		bool AlwaysAim = false;
		bool AutoReload = false;
		bool DebugDraw = false;
	}

	namespace Misc {
		bool FakeLag = false;
		bool FlyHack = false;
		bool FastHeal = true;
		bool SpiderMan = true;
		bool ShowVisState = false;
		bool SlideWalk = true;
		bool DoorKnocker = true;
		bool WalkOnWater = true;
		bool SwimOnGround = false;
		bool OnLadder = false;
		bool LOSBypass = false;
	}
}
