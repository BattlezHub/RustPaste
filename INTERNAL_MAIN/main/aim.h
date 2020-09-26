float GetFov(Vector2 Pos) {
	return Math::Calc2D_Dist(Vector2(Vars::User::ScreenWidth / 2, Vars::User::ScreenHeight / 2), Pos);
}
float GetFov(BasePlayer* Entity) {
	Vector2 ScreenPos;
	if (!LocalPlayer->WorldToScreen(Entity->GetPosition(), ScreenPos)) return 1000.f;
	return GetFov(ScreenPos);
}
float GetFov(Vector3 Pos) {
	Vector2 ScreenPos;
	if (!LocalPlayer->WorldToScreen(Pos, ScreenPos)) return 1000.f;
	return GetFov(ScreenPos);
}


float GetGravity(int ammoid) {
	switch (ammoid) {
	case 14241751: //fire arrow
		return 1.f;
	case -1234735557: //arrow normal
		return 0.75f;
	case 215754713: //arrow bone
		return 0.75f;
	case -1023065463: //hv arrow
		return 0.5f;
	case -2097376851: //nails
		return 0.75f;
	case -1321651331: //5.56 exp
		return 1.25f;
	default:
		return 1.f;
	}
}


double CalcBulletDrop(double height, double DepthPlayerTarget, float velocity, float gravity) {
	double pitch = (m_atan2(height, DepthPlayerTarget));
	double BulletVelocityXY = velocity * m_cos(pitch);
	double Time = DepthPlayerTarget / BulletVelocityXY;
	double TotalVerticalDrop = (0.5f * gravity * Time * Time);
	return TotalVerticalDrop * 10;
}

void Prediction(Vector3 local, Vector3& target, Vector3 targetvel, float bulletspeed, float gravity) {
	float Dist = Math::Calc3D_Dist(local, target);
	float BulletTime = Dist / bulletspeed;

	Vector3 vel = Vector3(targetvel.x, 0, targetvel.z) * 0.75f;

	Vector3 PredictVel = vel * BulletTime;

	target += PredictVel;

	double height = target.y - local.y;
	Vector3 dir = target - local;
	float DepthPlayerTarget = sqrtn(pow(dir.x) + pow(dir.z));

	float drop = CalcBulletDrop(height, DepthPlayerTarget, bulletspeed, gravity);
	//float drop = (4.905f * BulletTime * BulletTime) * gravity;
	target.y += drop;
}