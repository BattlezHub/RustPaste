bool mfound = false;
#define oGameObjectManager 0x17A6AD8  
void FindMatrix() {
	if (mfound) return;
	UINT64 ObjMgr = safe_read(GetModBase(xorstr(L"UnityPlayer.dll")) + oGameObjectManager, UINT64);
	UINT64 end = safe_read(ObjMgr, UINT64);
	for (UINT64 Obj = safe_read(ObjMgr + 0x8, UINT64); (Obj && (Obj != end)); Obj = safe_read(Obj + 0x8, UINT64))
	{
		UINT64 GameObject = safe_read(Obj + 0x10, UINT64);
		WORD Tag = safe_read(GameObject + 0x54, WORD);
		if (Tag == 5) {
			UINT64 ObjClass = safe_read(GameObject + 0x30, UINT64);
			UINT64 Entity = safe_read(ObjClass + 0x18, UINT64);
			pViewMatrix = (Matrix4x4*)(Entity + 0x2E4); //0xDC
			//printf(xorstr("Found matrix!, Entity 0x%llX\n"), Entity);
			mfound = true;
			return;
		}
	}
}
