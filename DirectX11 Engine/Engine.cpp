#include "Engine.h"

bool Engine::Init(HINSTANCE hInstance, std::string windowTitle, std::string windowClass, int width, int height)
{
	timer.Start();

	if (!this->renderWindow.Init(this, hInstance, windowTitle, windowClass, width, height))
		return false;

	if (!gfx.Init(renderWindow.GetHandle(), width, height))
		return false;

	return true;
}

bool Engine::ProcessMessages()
{
	return renderWindow.ProcessMessages();
}

void Engine::Update()
{
	float dt = timer.GetMilisecondsElapsed();

	while (!keyboard.CharBufferIsEmpty())
	{
		unsigned char ch = keyboard.ReadChar();

	}
	while (!keyboard.KeyBufferIsEmpty())
	{
		KeyboardEvent kbe = keyboard.ReadKey();
		unsigned char keycode = kbe.GetKeyCode();
	}
	while (!mouse.EventBufferIsEmpty())
	{
		MouseEvent me = mouse.ReadEvent();
		if (mouse.IsRightDown())
		{
			if (me.GetType() == MouseEvent::EventType::RAW_MOVE)
			{
				this->gfx.camera.AdjustRotation(static_cast<float>(me.GetPosY()) * 0.01f, static_cast<float>(me.GetPosX()) * 0.01f, 0.0f);
			}
		}
	}

	this->gfx.gObj.AdjustRotation(gfx.modelRotationSpeed[0] *dt, gfx.modelRotationSpeed[1] * dt, gfx.modelRotationSpeed[2] * dt);

	if (keyboard.KeyIsPrssed('W'))
	{
		this->gfx.camera.AdjustPosition(this->gfx.camera.GetForwardVector() * gfx.cameraSpeed * dt);
	}
	if (keyboard.KeyIsPrssed('A'))
	{
		this->gfx.camera.AdjustPosition(this->gfx.camera.GetLeftVector() * gfx.cameraSpeed * dt);
	}
	if (keyboard.KeyIsPrssed('S'))
	{
		this->gfx.camera.AdjustPosition(this->gfx.camera.GetBackwardVector() * gfx.cameraSpeed * dt);
	}
	if (keyboard.KeyIsPrssed('D'))
	{
		this->gfx.camera.AdjustPosition(this->gfx.camera.GetRightVector() * gfx.cameraSpeed * dt);
	}
	if (keyboard.KeyIsPrssed(VK_SPACE))
	{
		this->gfx.camera.AdjustPosition(0.0f, gfx.cameraSpeed * dt, 0.0f);
	}
	if (keyboard.KeyIsPrssed(VK_SHIFT))
	{
		this->gfx.camera.AdjustPosition(0.0f, -gfx.cameraSpeed * dt, 0.0f);
	}
	if (keyboard.KeyIsPrssed('C'))
	{
		XMVECTOR lightPosition = this->gfx.camera.GetPositionVector();
		lightPosition -= this->gfx.camera.GetForwardVector();
		this->gfx.pointLight.SetPosition(lightPosition);
		this->gfx.pointLight.SetRotation(this->gfx.camera.GetRotationFloat3());
	}

	timer.Restart();

}

void Engine::RenderFrame()
{
	gfx.RenderFrame();
}
