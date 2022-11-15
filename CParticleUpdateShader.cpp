#include "pch.h"
#include "CParticleUpdateShader.h"

#include "CDevice.h"
#include "CStructuredBuffer.h"

CParticleUpdateShader::CParticleUpdateShader()
	: CComputeShader(128,1,1)
	, m_ParticleBuffer(nullptr)
{
}

CParticleUpdateShader::~CParticleUpdateShader()
{
}

void CParticleUpdateShader::SetParticleBuffer(CStructuredBuffer* _Buffer)
{
	assert(_Buffer);

	m_ParticleBuffer = _Buffer; // 구조화버퍼 설정 -> 파티클은 너무 많기 때문에 구조화 버퍼에 넘겨야됌
	m_Param.iArr[0] = m_ParticleBuffer->GetElementCount(); // 갯수 iArr에 바인딩
}

void CParticleUpdateShader::UpdateData()
{
	assert(m_ParticleBuffer);

	m_ParticleBuffer->UpdateData_CS(0, false);

	// X 그룹에 할당할 갯수를 현재 해상도에서 Thread갯수로 나눠서 +1 한 상태로 할당, 
	m_iGroupX = m_Param.iArr[0] / m_iGroupPerThreadX + 1;
	m_iGroupY = 1;
	m_iGroupZ = 1;
}

void CParticleUpdateShader::Clear()
{
	m_ParticleBuffer->Clear();
}
