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
		/*
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
		*/
		//Update hitbox position.
		enemy->hitbox.min = (Vector3){-1.8f, -1.8f, -1.8f};
		enemy->hitbox.max = (Vector3){ 1.8f, 1.8f, 1.8f };
		Matrix ppos = MatrixTranslate(enemy->pos.x, enemy->pos.y, enemy->pos.z);
		enemy->hitbox.min = Vector3Transform(enemy->hitbox.min, ppos);
		enemy->hitbox.max = Vector3Transform(enemy->hitbox.max, ppos);
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
		Matrix r2 = MatrixRotate((Vector3){0.0f, 1.0f, 0.0f}, PI / 4.0f);
		//Matrix r3 = MatrixRotate(Vector3Normalize(Vector3Add((Vector3) { 0.0f, 1.0f, 0.0f }, enemy->pos)), 0);
		//Matrix r3 = MatrixRotate(Vector3Add((Vector3) { 0.0f, 1.0f, 0.0f }, enemy->pos), PI / 6.0f);
		r = MatrixMultiply(r, r2);
		Matrix t = MatrixTranslate(enemy->pos.x, enemy->pos.y, enemy->pos.z + 8);

		skullModel.transform = MatrixMultiply(MatrixMultiply(s, r), t);
		DrawModel(skullModel, Vector3Zero(), 1.0f, WHITE);
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
	e.right = (Vector3){ 1.0f, 0.0f, 0.0f };

	e.timer = 0.0f;

	switch (e.type)
	{
	case Skull:
	{
		e.health = 2;

		e.hitbox.min = (Vector3){ -1.8f, -1.8f, -1.8f };
		e.hitbox.max = (Vector3){ 1.8f, 1.8f, 1.8f };
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