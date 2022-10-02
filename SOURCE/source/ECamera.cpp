#include "ECamera.h"
#include "EMath.h"
#include "SDL.h"
#include <iostream>

Elite::Camera::Camera(const FPoint3& pos, float fovAngle) :
	m_FovAngle(fovAngle* static_cast<float>(E_PI) / 180.f),
	m_Position(pos),
	//m_Position(0.f,1.f, 5.f),
	m_LookAtMatrix{},
	m_Forward{ 0.f,0.f,-1.f },
	m_WorldUp{ 0.f,1.0f,0.f },
	m_Up{},
	m_Right{},
	m_PressesKeyButton{ false },
	m_PrevMousePos{ 0,0 },
	m_PrevMouseButton{ 0 },
	m_ChosenDistance{ 0.5f },
	m_FOV{ tan(m_FovAngle / 2.f) },
	m_PitchRotationMatrix{ },
	m_PrevJawAngle{0.f},
	m_PrevPitchAngle{0.f},
	m_MoveSpeed{0.1f}

{


	ResetLookAtMatrix();


}

void Elite::Camera::Update()
{
	const Uint8* state = SDL_GetKeyboardState(NULL);

	SDL_PumpEvents();  // make sure we have the latest mouse state.
	MousePos mousePos;
	Uint32 buttons;


	buttons = SDL_GetMouseState(&mousePos.x, &mousePos.y);

	//SDL_MouseWheelEvent

	if ((buttons & SDL_BUTTON_LMASK) != 0 && mousePos != m_PrevMousePos)
	{
		MoveForward((m_PrevMousePos.y - mousePos.y)* m_MoveSpeed);
		Pitch((mousePos.x - m_PrevMousePos.x) * m_MoveSpeed);
		m_PressesKeyButton = true;

	}
	else if ((buttons & SDL_BUTTON_MMASK) != 0 && mousePos != m_PrevMousePos)
	{
		MoveRight((mousePos.x - m_PrevMousePos.x) * m_MoveSpeed);
		MoveUp((m_PrevMousePos.y - mousePos.y) * 0.1f);
		m_PressesKeyButton = true;
	}
	else if ((buttons & SDL_BUTTON_RMASK) != 0 && mousePos != m_PrevMousePos)
	{
		Pitch((mousePos.x - m_PrevMousePos.x) * m_MoveSpeed);
		Yaw((m_PrevMousePos.y - mousePos.y)* m_MoveSpeed);
		m_PressesKeyButton = true;

	}

	if (state[SDL_SCANCODE_A] || state[SDL_SCANCODE_LEFT])
	{

		MoveRight(-m_ChosenDistance);
		m_PressesKeyButton = true;

	}
	else if (state[SDL_SCANCODE_D] || state[SDL_SCANCODE_RIGHT])
	{
		MoveRight(m_ChosenDistance);
		m_PressesKeyButton = true;
	}
	if (state[SDL_SCANCODE_W] || state[SDL_SCANCODE_UP])
	{
		MoveForward(m_ChosenDistance);
		m_PressesKeyButton = true;
	}
	else if (state[SDL_SCANCODE_S] || state[SDL_SCANCODE_DOWN])
	{
		MoveForward(-m_ChosenDistance);

		m_PressesKeyButton = true;
	}


	if (!m_PressesKeyButton)
	{
		m_PrevMousePos = mousePos;
		m_PrevMouseButton = 0;
		return;
	}


	CalculateLookAtMatrix();

	m_PrevMouseButton = buttons;
	m_PrevMousePos = mousePos;
	m_PressesKeyButton = false;
}

void Elite::Camera::Pitch(float angle)
{

	angle = ToRadians(angle);
	angle += m_PrevPitchAngle;

	m_PitchRotationMatrix = MakeRotation(angle, m_Up);

	m_PrevPitchAngle = angle;
}

void Elite::Camera::Yaw(float angle)
{
	angle = ToRadians(angle);
	angle += m_PrevJawAngle;

	m_JawRotationMatrix = MakeRotation(angle, m_Right);

	m_PrevJawAngle = angle;


}

void Elite::Camera::MoveRight(float distance)
{
	m_Position += GetNormalized(m_LookAtMatrix[0].xyz) * distance;

}

void Elite::Camera::MoveForward(float distance)
{

	m_Position += GetNormalized(m_LookAtMatrix[2].xyz) * distance;

}

void Elite::Camera::MoveUp(float distance)
{
	m_Position.y += distance;
}

void Elite::Camera::CalculateLookAtMatrix()
{
	
	

	m_Right = GetNormalized(Cross(m_WorldUp, m_Forward));
	m_Up = GetNormalized(Cross(m_Forward, m_Right));

	
	m_LookAtMatrix[0] = { m_Right.x, m_Right.y,m_Right.z, 0.f };
	m_LookAtMatrix[1] = { m_Up.x,m_Up.y,m_Up.z,0.f };
	m_LookAtMatrix[2] = { m_Forward.x,m_Forward.y,m_Forward.z,0.f };
	m_LookAtMatrix[3] = { m_Position.x,m_Position.y,m_Position.z,1.f };



	m_LookAtMatrix = m_LookAtMatrix * (m_PitchRotationMatrix * m_JawRotationMatrix);

}

void Elite::Camera::ResetLookAtMatrix()
{
	m_Right = GetNormalized(Cross(m_WorldUp, m_Forward));
	m_Up = GetNormalized(Cross(m_Forward, m_Right));

	m_LookAtMatrix[0] = { m_Right.x, m_Right.y,m_Right.z, 0.f };
	m_LookAtMatrix[1] = { m_Up.x,m_Up.y,m_Up.z,0.f };
	m_LookAtMatrix[2] = { m_Forward.x,m_Forward.y,m_Forward.z,0.f };
	m_LookAtMatrix[3] = { m_Position.x,m_Position.y,m_Position.z,1.f };

	m_PitchRotationMatrix[0] = { 1, 0,0,0 };
	m_PitchRotationMatrix[1] = { 0.f,cos(0.f), -sin(0.f),0.f };
	m_PitchRotationMatrix[2] = { 0,sin(0.f) , cos(0.f), 0 };
	m_PitchRotationMatrix[3] = { 0, 0,0,1 };


	m_JawRotationMatrix[0] = { cos(0.f),0.f, sin(0.f),0.f };
	m_JawRotationMatrix[1] = { 0.f,1.f, 0.f,0.f };
	m_JawRotationMatrix[2] = { -sin(0.f),0.f,cos(0.f), 0.f };
	m_JawRotationMatrix[3] = { 0.f, 0.f,0.f,1.f };
}

Elite::FMatrix4 Elite::Camera::GetLookAtMatrix() const
{
	return m_LookAtMatrix;
}

Elite::FPoint3 Elite::Camera::GetPosition() const
{
	return m_Position;
}

void Elite::Camera::SetCameraPos(const FPoint3& pos)
{
	m_Position = pos;
}

float Elite::Camera::GetFov() const
{
	return m_FOV;
}
