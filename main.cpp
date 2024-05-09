#define PI 3.14f
#include <Novice.h>
#include <cmath>
#include "Pipeline.h"
#include <imgui.h>

const char kWindowTitle[] = "LE2B_17_ナガイ_コハク_MT3_1_2 グリッドと球体の描画";

struct Sphere {
	Vector3 center; //中心点
	float radius; //半径
};

//ウィンドウサイズ
int kWindowWidth = 1280;
int kWindowHeight = 720;

void DrawGrid(const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix);
void DrawSphere(const Sphere& sphere, const Matrix4x4& viewProjectionMatrix, const Matrix4x4 viewportMatrix, uint32_t color);

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	Sphere sphere;

	sphere.center = { 0.0f,0.0f,0.0f };
	sphere.radius = 1.0f;

	Vector3 cameraTranslate{ 0.0f,1.9f,-6.49f };

	Vector3 cameraRotate{ 0.26f,0.0f,0.0f };

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		///
		/// ↓更新処理ここから
		///

		Matrix4x4 cameraMatrix = Pipeline::MakeAffineMatrix({ 1.0f,1.0f,1.0f }, cameraRotate, cameraTranslate);

		Matrix4x4 viewMatrix = Pipeline::Inverse(cameraMatrix);

		Matrix4x4 projectionMatrix = Pipeline::MakePerspectiveFovMatrix(0.45f, float(kWindowWidth) / float(kWindowHeight), 0.1f, 100.0f);

		Matrix4x4 viewProjectionMatrix = Pipeline::Multiply(viewMatrix, projectionMatrix);

		Matrix4x4 viewPortMatrix = Pipeline::MakeViewportMatrix(0, 0, float(kWindowWidth), float(kWindowHeight), 0.0f, 1.0f);

		ImGui::Begin("Window");
		ImGui::DragFloat3("CameraTranslate", &cameraTranslate.x, 0.01f);
		ImGui::DragFloat3("CameraRotate", &cameraRotate.x, 0.01f);
		ImGui::DragFloat3("ShpereCenter", &sphere.center.x, 0.01f);
		ImGui::DragFloat("SphereRadius", &sphere.radius, 0.01f);
		ImGui::End();

		DrawGrid(viewProjectionMatrix, viewPortMatrix);

		DrawSphere(sphere, viewProjectionMatrix, viewPortMatrix, 0x000000FF);

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		///
		/// ↑描画処理ここまで
		///

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}

void DrawGrid(const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix) {

	//Gridの半分の幅
	const float kGridHalfWidth = 2.0f;

	//分割数
	const uint32_t kSubdivision = 10;

	//1つ分の長さ
	const float kGridEvery = (kGridHalfWidth * 2.0f) / float(kSubdivision);

	Matrix4x4 worldMatrix = Pipeline::MakeAffineMatrix(
		Vector3(1, 1, 1),
		Vector3(0, 0, 0),
		Vector3(0, 0, 0)
	);

	Matrix4x4 worldViewProjectionMatrix = Pipeline::Multiply(worldMatrix, viewProjectionMatrix);

	//奥から手前への線を順順で引いていく
	for (uint32_t xIndex = 0; xIndex <= kSubdivision; ++xIndex) {

		float start = -(kSubdivision / 2.0f) * kGridEvery;
		float end = (kSubdivision / 2.0f) * kGridEvery;

		Vector3 startNdcVertex = Pipeline::Transform(Vector3(((kSubdivision / 2.0f) - xIndex) * kGridEvery, 0, start), worldViewProjectionMatrix);

		Vector3 startScreenVertex = Pipeline::Transform(startNdcVertex, viewportMatrix);

		Vector3 endNdcVertex = Pipeline::Transform(Vector3(((kSubdivision / 2.0f) - xIndex) * kGridEvery, 0, end), worldViewProjectionMatrix);

		Vector3 endScreenVertex = Pipeline::Transform(endNdcVertex, viewportMatrix);

		if (xIndex == kSubdivision / 2.0f) {
			Novice::DrawLine(
				int(startScreenVertex.x),
				int(startScreenVertex.y),
				int(endScreenVertex.x),
				int(endScreenVertex.y),
				0x000000FF);
		} else {
			Novice::DrawLine(
				int(startScreenVertex.x),
				int(startScreenVertex.y),
				int(endScreenVertex.x),
				int(endScreenVertex.y),
				0xAAAAAAFF);
		}

	}

	//左から右も同じように順々に引いていく
	for (uint32_t zIndex = 0; zIndex <= kSubdivision; ++zIndex) {

		float start = -(kSubdivision / 2.0f) * kGridEvery;
		float end = (kSubdivision / 2.0f) * kGridEvery;

		Vector3 startNdcVertex = Pipeline::Transform(Vector3(start, 0, ((kSubdivision / 2.0f) - zIndex) * kGridEvery), worldViewProjectionMatrix);

		Vector3 startScreenVertex = Pipeline::Transform(startNdcVertex, viewportMatrix);

		Vector3 endNdcVertex = Pipeline::Transform(Vector3(end, 0, ((kSubdivision / 2.0f) - zIndex) * kGridEvery), worldViewProjectionMatrix);

		Vector3 endScreenVertex = Pipeline::Transform(endNdcVertex, viewportMatrix);

		if (zIndex == kSubdivision / 2.0f) {
			Novice::DrawLine(
				int(startScreenVertex.x),
				int(startScreenVertex.y),
				int(endScreenVertex.x),
				int(endScreenVertex.y),
				0x000000FF);
		} else {
			Novice::DrawLine(
				int(startScreenVertex.x),
				int(startScreenVertex.y),
				int(endScreenVertex.x),
				int(endScreenVertex.y),
				0xAAAAAAFF);
		}
	}
}

void DrawSphere(const Sphere& sphere, const Matrix4x4& viewProjectionMatrix, const Matrix4x4 viewportMatrix, uint32_t color) {

	const uint32_t kSubDivision = 12;

	//経度1つ分の角度(φ)
	const float kLonEvery = (2.0f * PI) / kSubDivision;

	//緯度1つ分の角度(θ)
	const float kLatEvery = PI / kSubDivision;

	Matrix4x4 worldMatrix = Pipeline::MakeAffineMatrix(
		Vector3(1, 1, 1),
		Vector3(0, 0, 0),
		Vector3(0, 0, 0)
	);

	Matrix4x4 worldViewProjectionMatrix = Pipeline::Multiply(worldMatrix, viewProjectionMatrix);

	//緯度(θ)の方向に分割
	for (uint32_t latIndex = 0; latIndex < kSubDivision; ++latIndex) {

		float lat = -PI / 2.0f + kLatEvery * latIndex;

		//経度(φ)の方向に分割
		for (uint32_t lonIndex = 0; lonIndex < kSubDivision; ++lonIndex) {

			float lon = lonIndex * kLonEvery;

			//world座標系でのa,b,cを決める
			Vector3 a, b, c;

			a = { sphere.radius * cosf(lat) * cosf(lon)             , sphere.radius * sinf(lat)             , sphere.radius * cosf(lat) * sinf(lon) };
			b = { sphere.radius * cosf(lat + kLatEvery) * cosf(lon) , sphere.radius * sinf(lat + kLatEvery) , sphere.radius * cosf(lat + kLatEvery) * sinf(lon) };
			c = { sphere.radius * cosf(lat) * cosf(lon + kLonEvery) , sphere.radius * sinf(lat)             , sphere.radius * cosf(lat) * sinf(lon + kLonEvery) };

			a.x += sphere.center.x;
			a.y += sphere.center.y;
			a.z += sphere.center.z;

			b.x += sphere.center.x;
			b.y += sphere.center.y;
			b.z += sphere.center.z;

			c.x += sphere.center.x;
			c.y += sphere.center.y;
			c.z += sphere.center.z;

			Vector3 ANdcVertex = Pipeline::Transform(a, worldViewProjectionMatrix);
			Vector3 BNdcVertex = Pipeline::Transform(b, worldViewProjectionMatrix);
			Vector3 CNdcVertex = Pipeline::Transform(c, worldViewProjectionMatrix);

			Vector3 AScreenVertex = Pipeline::Transform(ANdcVertex, viewportMatrix);
			Vector3 BScreenVertex = Pipeline::Transform(BNdcVertex, viewportMatrix);
			Vector3 CScreenVertex = Pipeline::Transform(CNdcVertex, viewportMatrix);

			Novice::DrawLine(
				int(AScreenVertex.x),
				int(AScreenVertex.y),
				int(BScreenVertex.x),
				int(BScreenVertex.y),
				color
			);

			Novice::DrawLine(
				int(AScreenVertex.x),
				int(AScreenVertex.y),
				int(CScreenVertex.x),
				int(CScreenVertex.y),
				color
			);

		}
	}
}