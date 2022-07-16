#include "Globals.h"
#include "Enemies.h"
#include "raylib/raymath.h"
#include "stdio.h"

void SetupEnemies(void)
{
	skullModel = LoadModel("resources/Models/Skull/skull_monster.obj");
}

void UpdateEnemy(Enemy* enemy)
{
	switch (enemy->type)
	{
	case Skull:
	{
		enemy->timer += dt;
		if (enemy->timer >= 3.0f)
		{
			enemy->timer = 0.0f;
			enemy->goal = player.camera.position;
		}

		//Update the orientational vectors and the position.
		Vector3 dirToGoal = Vector3Normalize(Vector3Subtract(enemy->goal, enemy->pos));
		Vector3 dirToGoalRight = Vector3Normalize(Vector3CrossProduct(dirToGoal, (Vector3) { 0.0f, 1.0f, 0.0f }));
		Vector3 dirToGoalUp = Vector3Normalize(Vector3CrossProduct(dirToGoal, dirToGoalRight));

		Vector3 diff = Vector3Subtract(dirToGoal, enemy->forward);
		Vector3 diffRight = Vector3Subtract(dirToGoalRight, enemy->right);
		Vector3 diffUp = Vector3Subtract(dirToGoalUp, enemy->up);

		enemy->forward = Vector3Normalize(Vector3Add(enemy->forward, Vector3Scale(diff, dt)));
		enemy->up = Vector3Normalize(Vector3Add(enemy->up, Vector3Scale(diffRight, dt)));
		enemy->right = Vector3Normalize(Vector3Add(enemy->right, Vector3Scale(diffUp, dt)));

		enemy->pos = Vector3Add(enemy->pos, Vector3Scale(enemy->forward, enemy->speed * dt));
		break;
	}
	default:
	{

	}
	}
}

void DrawEnemy(Enemy* enemy)
{
	switch (enemy->type)
	{
	case Skull:
	{
		skullModel.transform = MatrixIdentity();
		Matrix s = MatrixScale(enemy->scale.x, enemy->scale.y, enemy->scale.z);
		Matrix r = MatrixLookAt(enemy->pos, enemy->forward, enemy->up);
		Matrix t = MatrixTranslate(enemy->pos.x, enemy->pos.y, enemy->pos.z);

		skullModel.transform = MatrixMultiply(MatrixMultiply(s, r), t);
		DrawModel(skullModel, Vector3Zero(), 1.0f, WHITE);
		break;
	}
	default:
	{
		break;
	}
	}
}

Enemy CreateEnemy(EnemyType type, Vector3 startPos, Vector3 startGoal, Vector3 startScale, float enemySpeed)
{
	Enemy e = { 0 };
	e.type = type;
	e.pos = startPos;
	e.goal = startGoal;
	e.scale = startScale;
	e.speed = enemySpeed;

	e.forward = (Vector3){0.0f, 0.0f, 1.0f};
	e.up = (Vector3){ 0.0f, 1.0f, 0.0f };
	e.right = (Vector3){ 1.0f, 0.0f, 0.0f };

	e.timer = 0.0f;

	switch (e.type)
	{
	case Skull:
	{
		e.health = 2;
		break;
	}
	default:
	{
		break;
	}
	}

	return e;
}