
typedef enum GunType{
	Pistol = 0,
	SMG,
	Shotgun
} GunType;

typedef struct Gun
{
	GunType currentGun;
	unsigned currentAmmo;
	unsigned maxAmmo;
	float shootCD;
	float shootTimer;
	Vector3 pos;
	float rot;
} Gun;

void SetupGun(Gun* gun);
void ChangeGun(Gun* gun, GunType type);
void UpdateGun(Gun* gun, Camera playerCamera);
void Shoot(Gun* gun);
void DrawGun(Gun* gun);

Model pistolModel;
Model smgModel;
Model shotgunModel;