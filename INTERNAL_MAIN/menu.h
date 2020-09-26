void OnKeyUp(KeyCode key) {
	if (key == KeyCode::Insert) {
		Vars::User::InsertPressed = false;
	}
}
void OnKeyDown(KeyCode key) {
	if (!Vars::User::InsertPressed && key == KeyCode::Insert) {
		Vars::User::InsertPressed = true;
		Vars::User::MenuOpen = !Vars::User::MenuOpen;
	}
	if (key == KeyCode::End) {
		Vars::User::Panic = true;
	}
}

void DoTab(EventType event, Vector2 menupos, Vector2 mouse, const wchar_t* name, int id, Vector2 tab_size = Vector2(100.f, 40.f)) {
	if (event == EventType::MouseDown) {
		if (Rect(menupos.x, menupos.y + tab_size.y * id, tab_size.x, tab_size.y).Contains(mouse)) {
			Vars::User::activetab = id;
		}
	}

	if (event == EventType::Repaint) {
		if (Vars::User::activetab == id) {
			GUI::VerticalLine(Vector2(menupos.x + 2.f, menupos.y + id * tab_size.y), tab_size.y, Color(0, 1, 0.95, 1), 1.5f);
		}
		GUI::Label(Rect(menupos.x, menupos.y + id * tab_size.y, tab_size.x, tab_size.y), Str(name), Color(1, 1, 1, 1), true);
	}
}
void DoButton(EventType event, Vector2 menupos, Vector2& cur, Vector2 mouse, const wchar_t* name, bool* IO) {
	const float button_size = 14.f;
	if (event == EventType::MouseDown) {
		if (Rect(menupos.x + cur.x, menupos.y + cur.y, button_size, button_size).Contains(mouse)) {
			*IO = !*IO;
		}
	}

	if (event == EventType::Repaint) {
		GUI::Box(Vector2(menupos.x + cur.x, menupos.y + cur.y), Vector2(button_size, button_size), Color(0, 1, 0.72, 1));
		GUI::Label(Rect(menupos.x + cur.x + button_size + 3.f, menupos.y + cur.y, 200, button_size + 3.f), Str(name), Color(1, 1, 1, 1), false, 10.f);

		if (*IO) {
			GUI::FillBox(Rect(menupos.x + cur.x, menupos.y + cur.y, button_size, button_size), Color(0, 1, 0.72, 1));
		}
	}
	cur.y += button_size + 5.f;
}

typedef Vector3(__stdcall* mm)();
void MenuTick(DWORD64 Event, EventType type) {
	if (type == EventType::KeyDown || type == EventType::KeyUp) {
		typedef KeyCode(__stdcall* KK)(DWORD64);
		KeyCode cur = ((KK)(g_Base + O::UnityEngine_Event::get_keyCode))(Event);
 		if (type == EventType::KeyDown) OnKeyDown(cur);
		else OnKeyUp(cur);
	}
	if (!Vars::User::MenuOpen || Vars::User::Panic) return;

	Vector2 curpos = Vector2(); 
	Vector2 menupos = Vector2(650, 200);
	Vector2 menusize = Vector2(700, 400);
	Vector2 buttonSize = Vector2(200, 0/*unused*/);


	mm _get_mm = (mm)(g_Base + O::UnityEngine_Input::get_mousePosition);
	Vector3 _mm = _get_mm();
	Vector2 mouse = Vector2(_mm.x, Vars::User::ScreenHeight - _mm.y);

	GUI::FillBox(Rect(menupos.x, menupos.y, menusize.x, menusize.y), Color(0, 0, 0, 1));//main menu box

	GUI::FillBox(Rect(menupos.x, menupos.y, menusize.x, 30.f), Color(0.3, 0.3, 0.3, 1));

	GUI::Label(Rect(menupos.x, menupos.y, menusize.x, 30.f), Str(xorstr(L"timoxa5651WARE")), Color(1, 1, 1, 1), true, 16.f);
	menupos.y += 30.f;
	menusize.y -= 30.f;

	DoTab(type, menupos, mouse, xorstr(L"tab 1"), 0);
	DoTab(type, menupos, mouse, xorstr(L"tab 2"), 1);
	DoTab(type, menupos, mouse, xorstr(L"tab 3"), 2);
	DoTab(type, menupos, mouse, xorstr(L"tab 4"), 3);
	menupos += Vector2(120, 30);
	menusize.x -= 120;
	menusize.y -= 30; //tabs end, now functional
	

	if (Vars::User::activetab == 0) {
		DoButton(type, menupos, curpos, mouse, xorstr(L"Shoot Through terrain"), &Vars::Aim::Silent::Enabled);
		DoButton(type, menupos, curpos, mouse, xorstr(L"Psilent Aim"), &Vars::Aim::pSilent);
		DoButton(type, menupos, curpos, mouse, xorstr(L"Fat Bullet"), &Vars::Weapon::FatBullet);
		DoButton(type, menupos, curpos, mouse, xorstr(L"Aim On Wounded"), &Vars::Aim::AimOnWounded);
		DoButton(type, menupos, curpos, mouse, xorstr(L"Aim On Sleepers"), &Vars::Aim::AimOnSleepers);
		curpos.y += 20.f;
		DoButton(type, menupos, curpos, mouse, xorstr(L"Shoot 80m"), &Vars::Aim::Silent::Shoot80);
		DoButton(type, menupos, curpos, mouse, xorstr(L"Shoot 150m"), &Vars::Aim::Silent::Shoot150);
		DoButton(type, menupos, curpos, mouse, xorstr(L"Shoot 300m"), &Vars::Aim::Silent::Shoot300);
		curpos.y += 20.f;
		DoButton(type, menupos, curpos, mouse, xorstr(L"Aim Head"), &Vars::Aim::AimHead);
		DoButton(type, menupos, curpos, mouse, xorstr(L"Aim Body"), &Vars::Aim::AimBody);
		curpos.y += 10.f;
		DoButton(type, menupos, curpos, mouse, xorstr(L"Melee Aim Players"), &Vars::Aim::MeleeAimPlayers);
		DoButton(type, menupos, curpos, mouse, xorstr(L"Shooting Through Walls"), &Vars::Misc::LOSBypass);
	}

	else if (Vars::User::activetab == 1) {
		DoButton(type, menupos, curpos, mouse, xorstr(L"Draw Box"), &Vars::PlayerEsp::Box);
		DoButton(type, menupos, curpos, mouse, xorstr(L"Draw nickname"), &Vars::PlayerEsp::DrawNick);
		DoButton(type, menupos, curpos, mouse, xorstr(L"Draw HP"), &Vars::PlayerEsp::DrawHpBar);
		DoButton(type, menupos, curpos, mouse, xorstr(L"Draw Weapon"), &Vars::PlayerEsp::DrawWeapon);
		DoButton(type, menupos, curpos, mouse, xorstr(L"Crosshair"), &Vars::Visuals::Crosshair);
	}
	
	else if (Vars::User::activetab == 2) {
		DoButton(type, menupos, curpos, mouse, xorstr(L"Fake Lag"), &Vars::Misc::FakeLag);
		DoButton(type, menupos, curpos, mouse, xorstr(L"650+ dist))"), &Vars::Aim::FakeDist);
		DoButton(type, menupos, curpos, mouse, xorstr(L"No Recoil"), &Vars::Weapon::NoRecoil);
		DoButton(type, menupos, curpos, mouse, xorstr(L"No Sway"), &Vars::Weapon::NoSway);
		DoButton(type, menupos, curpos, mouse, xorstr(L"Super Eoka"), &Vars::Weapon::SuperEoka);
		DoButton(type, menupos, curpos, mouse, xorstr(L"Automatic"), &Vars::Weapon::Automatic);
		//	DoButton(type, menupos, curpos, mouse, xorstr(L"узнать почему ниху€ не работает"), &Vars::Weapon::DebugDraw);
		DoButton(type, menupos, curpos, mouse, xorstr(L"Show Vis State"), &Vars::Misc::ShowVisState);
	}
	else if (Vars::User::activetab == 3) {
		DoButton(type, menupos, curpos, mouse, xorstr(L"Logo"), &Vars::PlayerEsp::Logo);
		DoButton(type, menupos, curpos, mouse, xorstr(L"Melee Auto Farm"), &Vars::Aim::MeleeAutoFarm);
		DoButton(type, menupos, curpos, mouse, xorstr(L"Melee Pickup"), &Vars::Aim::MeleePickup);
		DoButton(type, menupos, curpos, mouse, xorstr(L"SpiderMan"), &Vars::Misc::SpiderMan);
		DoButton(type, menupos, curpos, mouse, xorstr(L"FastHeal"), &Vars::Misc::FastHeal);
		DoButton(type, menupos, curpos, mouse, xorstr(L"SlideWalk"), &Vars::Misc::SlideWalk);
		DoButton(type, menupos, curpos, mouse, xorstr(L"Door Knocker"), &Vars::Misc::DoorKnocker);
		DoButton(type, menupos, curpos, mouse, xorstr(L"WalkOnWater"), &Vars::Misc::WalkOnWater);
		DoButton(type, menupos, curpos, mouse, xorstr(L"Swim On Ground"), &Vars::Misc::SwimOnGround);
		DoButton(type, menupos, curpos, mouse, xorstr(L"Fly Hack"), &Vars::Misc::FlyHack);
	}
}