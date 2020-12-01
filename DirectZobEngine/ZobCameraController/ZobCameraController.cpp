#include "ZobCameraController.h"


ZobCameraController::ZobCameraController(Camera * c)
{
	m_type = Camera::eCamera_base;
    m_zobCamera = c;
}

ZobCameraController::~ZobCameraController()
{
    m_zobCamera = NULL;
}

void ZobCameraController::Update()
{
}

void ZobCameraController::Rotate(float x, float y, float z)
{
}

void ZobCameraController::Move(float x, float y, float z)
{
}

