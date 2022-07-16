#pragma once

typedef enum EnemyType
{
	Skull = 0,
	Temp
} EnemyType;

typedef struct Enemy
{
	EnemyType type;
	unsigned health;
	Vector3 pos;
	Vector3 up;
	Vector3 forward;
	Vector3 right;
	Vector3 scale;
	Vector3 goal; //Position the enemy is moving towards.
	float timer;
	float speed;
} Enemy;

void SetupEnemies(void);

void UpdateEnemy(Enemy* enemy);

void DrawEnemy(Enemy* enemy);

Enemy CreateEnemy(EnemyType type, Vector3 startPos, Vector3 startGoal, Vector3 startScale, float enemySpeed);

Model skullModel;