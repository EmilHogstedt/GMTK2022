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
		if (enemy->timer >= 1.0f)
		{
			enemy->timer = 0.0f;
			enemy->goal = player.camera.position;

			enemy->lerpDir1 = enemy->forward;
			enemy->lerpDir2 = Vector3Normalize(Vector3Subtract(enemy->goal, enemy->pos));
		}
		
		//Update the orientational vectors and the position.
		Vector3 dirToGoal = Vector3Normalize(Vector3Subtract(enemy->goal, (Vector3) { skullModel.transform.m12, skullModel.transform.m13, skullModel.transform.m14 }));
		Vector3 dirToGoalRight = Vector3Normalize(Vector3CrossProduct(dirToGoal, (Vector3) { 0.0f, 1.0f, 0.0f }));
		Vector3 dirToGoalUp = Vector3Normalize(Vector3CrossProduct(dirToGoal, dirToGoalRight));

		Vector3 diff = Vector3Subtract(dirToGoal, enemy->forward);
		Quaternion q = QuaternionFromVector3ToVector3(enemy->forward, dirToGoal);
		q = QuaternionLerp(QuaternionFromEuler(0.0f, 0.0f, 0.0f), q, enemy->timer / 1.0f);
		enemy->forward = Vector3Normalize(Vector3RotateByQuaternion(enemy->forward, q));
		
		/*
		float t = Vector3Length(diff);
		if (t >= 0.2f && t <= 1.0f)
		{
			Vector3 diffRight = Vector3Subtract(dirToGoalRight, enemy->right);
			Vector3 diffUp = Vector3Subtract(dirToGoalUp, enemy->up);

			enemy->forward = Vector3Normalize(Vector3Add(enemy->forward, Vector3Scale(diff, enemy->timer / 6.0f)));
			enemy->up = Vector3Normalize(Vector3Add(enemy->up, Vector3Scale(diffRight, enemy->timer / 6.0f)));
			enemy->right = Vector3Normalize(Vector3Add(enemy->right, Vector3Scale(diffUp, enemy->timer / 6.0f)));
		}
		printf("%f\n", t);
		
		*/

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
		//Matrix r2 = MatrixRotate((Vector3){1.0f, 0.0f, 0.0f}, PI / 4.0f);
		//Matrix r3 = MatrixRotate((Vector3) { 0.0f, 0.0f, 1.0f }, PI / 8.0f);
		//Matrix r3 = MatrixRotate(Vector3Add((Vector3) { 0.0f, 1.0f, 0.0f }, enemy->pos), PI / 6.0f);
		//r = MatrixMultiply(MatrixMultiply(r2, r3), r);
		Matrix t = MatrixTranslate(enemy->pos.x - 1.4, enemy->pos.y + 5, enemy->pos.z);

		skullModel.transform = MatrixMultiply(MatrixMultiply(s, r), t);
		DrawModel(skullModel, Vector3Zero(), 1.0f, WHITE);

		enemy->hitbox.min = (Vector3){ -1.8f * enemy->scale.x, -2.5f * enemy->scale.y, -1.8f * enemy->scale.z };
		enemy->hitbox.max = (Vector3){ 1.8f * enemy->scale.x, 2.5f * enemy->scale.y, 1.8f * enemy->scale.z };
		Matrix ppos = MatrixTranslate(skullModel.transform.m12, skullModel.transform.m13, skullModel.transform.m14);
		enemy->hitbox.min = Vector3Transform(enemy->hitbox.min, ppos);
		enemy->hitbox.max = Vector3Transform(enemy->hitbox.max, ppos);

		DrawSphere(enemy->goal, 3, RED);

		DrawLine3D((Vector3) { skullModel.transform.m12, skullModel.transform.m13, skullModel.transform.m14 }, Vector3Add((Vector3) { skullModel.transform.m12, skullModel.transform.m13, skullModel.transform.m14 }, Vector3Scale(enemy->forward, 5.0f)), RED);
		break;
	}
	default:
	{
		break;
	}
	}

	DrawBoundingBox(enemy->hitbox, GREEN);
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
	e.right = (Vector3){ -1.0f, 0.0f, 0.0f };
	
	e.timer = 0.0f;
	e.lerpDir1 = e.forward;
	e.lerpDir2 = e.forward;

	switch (e.type)
	{
	case Skull:
	{
		e.health = 2;

		e.hitbox.min = (Vector3){ -1.8f * e.scale.x, -2.5f * e.scale.y, -1.8f * e.scale.z };
		e.hitbox.max = (Vector3){ 1.8f * e.scale.x, 2.5f * e.scale.y, 1.8f * e.scale.z };
		Matrix ppos = MatrixTranslate(e.pos.x, e.pos.y, e.pos.z);
		e.hitbox.min = Vector3Transform(e.hitbox.min, ppos);
		e.hitbox.max = Vector3Transform(e.hitbox.max, ppos);
		break;
	}
	default:
	{
		break;
	}
	}
	return e;
}