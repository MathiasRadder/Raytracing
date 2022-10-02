#pragma once
#include "EMath.h"

struct MousePos
{
	int x{0};
	int y{0};


	bool operator ==(const MousePos& a)
	{
	
		return x == a.x && y == a.y;
	}

	bool operator !=(const MousePos& a)
	{
		if (x != a.x)
		{
			return true;
		}
		else if (y != a.y)
		{
			return true;
		}
		else
		{
			return false;
		}
		
	}
};


namespace Elite
{
	typedef __int32  Uint32;
	class Camera
	{
	public:
		Camera(const FPoint3& pos, float fovAngle);
		void Update();

		FMatrix4 GetLookAtMatrix() const;
		FPoint3 GetPosition() const;
		void SetCameraPos(const FPoint3& pos);
		float GetFov()const;
	/*void LookAtFunction*/
	private:
		void Pitch(float angle);
		void Yaw(float angle);
		void MoveRight(float distance);
		void MoveForward(float distance);
		void MoveUp(float distance);
		void CalculateLookAtMatrix();
		void ResetLookAtMatrix();

		const float m_FovAngle;
		const float m_FOV;
		FPoint3 m_Position;
		FMatrix4 m_LookAtMatrix;
		FVector3 m_Forward;
		const FVector3 m_WorldUp;
		FVector3 m_Up;
		FVector3 m_Right;
		bool m_PressesKeyButton;
		MousePos m_PrevMousePos;
		Uint32 m_PrevMouseButton;
		const float m_ChosenDistance;
		FMatrix4 m_PitchRotationMatrix;
		FMatrix4 m_JawRotationMatrix;
		float m_PrevJawAngle;
		float m_PrevPitchAngle;
		const float m_MoveSpeed;
		

	};
}

