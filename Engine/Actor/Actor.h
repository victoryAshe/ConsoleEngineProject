#pragma once

#include "Common/RTTI.h"
#include "Math/Vector2.h"
#include "Math/Color.h"

namespace Wanted
{
	class Level;

	class WANTED_API Actor: public RTTI
	{
		// Add RTTI code
		RTTI_DECLARATIONS(Actor, RTTI)

	public:

		// 기존 코드와 호환성 유지를 위한 1줄 ASCII charString 생성자.
		Actor(
			const char* image = "",
			const Vector2& position = Vector2::Zero,
			Color color = Color::White
			);

		// 2D ASCII Art 생성자.
		Actor(
			const char* image,
			int inWidth,
			int inHeight,
			const Vector2& position,
			Color color = Color::White,
			bool spaceTransparent = true
		);

		virtual ~Actor();

		// GamePlay Event.
		virtual void BeginPlay();
		virtual void Tick(float deltaTime);
		virtual void Draw();

		// 삭제 요청 함수.
		void Destroy();

		// 삭제가 될 때 호출될 event 함수.
		virtual void OnDestroy();

		// 게임 종료 함수.
		void QuitGame();

		// 충돌 여부 확인 함수.
		bool TestIntersect(const Actor* const other);

		// one-line-Actor의 Image값 변경 함수.
		void ChangeImage(const char* newImage);

		// 2D Actor의 Image값 변경 함수.
		void ChangeSprite(
			const char* newImage,
			int inWidth,
			int inHeight,
			bool inSpaceTransparent = true
		);

		// 위치 변경 및 읽기 함수.
		void SetPosition(const Vector2& newPosition);
		inline Vector2 GetPosition() const { return position; }

		// Add/Read Ownership.
		inline void SetOwner(Level* newOwner) { owner = newOwner; }
		inline Level* GetOwner() const { return owner; }

		// Getter.
		inline bool HasBeganPlay() const 
		{ 
			return hasBeganPlay; 
		}
		
		inline bool IsActive() const
		{ 
			return isActive && !destroyRequested;
		}

		inline bool DestroyRequested() const 
		{ 
			return destroyRequested; 
		}

		inline int GetSortingOrder() const { return sortingOrder; }

		inline int GetWidth() const { return width; }
		inline int GetHeight() const { return height; }

		inline bool IsSpaceTransparent() const { return spaceTransparent; }

	protected:
		// 이미 BeginPlay event를 받았는지 여부.
		bool hasBeganPlay = false;

		// 활성화 상태 여부.
		bool isActive = true;

		// 현재 frame에 삭제 요청 받았는지 여부.
		bool destroyRequested = false;

		// 내부 저장
		// 1줄: [width+1]
		// 2D: [(width+1)*height]  
		// 각 row 마지막 'null'\0'을 보장.
		char* image = nullptr;

		int width = 0;
		int height = 0;

		// ASCII 공백의 투명도를 인정
		// true=> 공백인 곳에 sortingOrder가 뒤에 있는 동일위치의 Actor의 image를 합성
		bool spaceTransparent = true;

		// 색상
		Color color = Color::White;

		// Ownership.
		Level* owner = nullptr;

		// Render priority: 값이 크면 우선순위가 높도록 설정.
		int sortingOrder = 0;

		// 위치.
		Vector2 position;
	};

}

