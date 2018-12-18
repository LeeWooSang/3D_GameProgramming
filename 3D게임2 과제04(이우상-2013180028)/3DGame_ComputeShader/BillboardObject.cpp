#include "Stdafx.h"
#include "BillboardObject.h"

CBillboardObject::CBillboardObject()
{
}

CBillboardObject::~CBillboardObject()
{
}

void CBillboardObject::Animate(float fTimeElapsed)
{
	if (m_fRotationAngle <= -1.5f) m_fRotationDelta = 1.0f;
	if (m_fRotationAngle >= +1.5f) m_fRotationDelta = -1.0f;
	m_fRotationAngle += m_fRotationDelta * fTimeElapsed;

	Rotate(0.0f, 0.0f, m_fRotationAngle);
}