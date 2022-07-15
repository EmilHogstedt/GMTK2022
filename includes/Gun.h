
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
	Matrix matrix;
} Gun;

void SetupGun(Gun* gun);
void ChangeGun(Gun* gun, GunType type);
void UpdateGun(Gun* gun, Vector3 playerPos, Vector3 playerForward);
void Shoot(Gun* gun);
void DrawGun(Gun* gun);