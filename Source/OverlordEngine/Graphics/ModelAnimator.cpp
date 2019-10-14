//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"

#include "ModelAnimator.h"
#include "../Diagnostics/Logger.h"


ModelAnimator::ModelAnimator(MeshFilter* pMeshFilter):
m_pMeshFilter(pMeshFilter),
m_Transforms(vector<XMFLOAT4X4>()),
m_IsPlaying(false), 
m_Reversed(false),
m_ClipSet(false),
m_TickCount(0),
m_AnimationSpeed(1.0f)
{
	SetAnimation(0);
}


ModelAnimator::~ModelAnimator()
{
}

void ModelAnimator::SetAnimation(UINT clipNumber)
{
	//Set m_ClipSet to false
	m_ClipSet = false;
	//Check if clipNumber is smaller than the actual m_AnimationClips vector size
	if (clipNumber < GetClipCount())
	{
		//else
		//	Retrieve the AnimationClip from the m_AnimationClips vector based on the given clipNumber
		//	Call SetAnimation(AnimationClip clip)
		SetAnimation(m_pMeshFilter->m_AnimationClips[clipNumber]);
	}
	else
	{
		//If not,
		//	Call Reset
		Reset();
		//	Log a warning with an appropriate message
		Logger::LogFormat(LogLevel::Warning, L"clipNumber is not within the index range of m_AnimationClips!\n");
		//	return
		return;
	}

}

void ModelAnimator::SetAnimation(wstring clipName)
{
	//Set m_ClipSet to false
	m_ClipSet = false;
	//Iterate the m_AnimationClips vector and search for an AnimationClip with the given name (clipName)
	for (const AnimationClip &clip : m_pMeshFilter->m_AnimationClips)
	{
		if (clip.Name.compare(clipName) == 0)
		{
			//If found,
			//	Call SetAnimation(Animation Clip) with the found clip
			SetAnimation(clip);
		}
		else
		{
			
			//Else
			//	Call Reset
			Reset();
			//	Log a warning with an appropriate message
			Logger::LogFormat(LogLevel::Warning, clipName.c_str(), L" is not found in m_AnimationClips!\n");
			//return
			return;
		}
	}


}

void ModelAnimator::SetAnimation(AnimationClip clip)
{
	//Set m_ClipSet to true
	m_ClipSet = true;
	//Set m_CurrentClip
	m_CurrentClip = clip;
	//Call Reset(false)
	Reset(false);
}

void ModelAnimator::Reset(bool pause)
{
	//If pause is true, set m_IsPlaying to false
	pause == true ? m_IsPlaying = false : m_IsPlaying = true;
	
	//Set m_TickCount to zero
	m_TickCount = 0;
	//Set m_AnimationSpeed to 1.0f
	m_AnimationSpeed = 1.0f;
	
	if (m_ClipSet == true)
	{
		//If m_ClipSet is true
		//	Retrieve the BoneTransform from the first Key from the current clip (m_CurrentClip)
		auto BoneTransforms = m_CurrentClip.Keys[0].BoneTransforms;
		//	Refill the m_Transforms vector with the new BoneTransforms (have a look at vector::assign)
		m_Transforms.assign(BoneTransforms.begin(), BoneTransforms.end());
	}
	else
	{
		//Else
		//	Create an IdentityMatrix 
		XMFLOAT4X4 identityMatrix;
		XMStoreFloat4x4(&identityMatrix, XMMatrixIdentity());
		//	Refill the m_Transforms vector with this IdenityMatrix (Amount = BoneCount) (have a look at vector::assign)
		m_Transforms.assign(GetBoneTransforms().size(), identityMatrix);
	}

}

void ModelAnimator::Update(const GameContext& gameContext)
{
	//We only update the transforms if the animation is running and the clip is set
	if (m_IsPlaying && m_ClipSet)
	{
		float clipDuration{ m_CurrentClip.Duration };

		//1. 
		//Calculate the passedTicks (see the lab document)
		//auto passedTicks = ...
		//Make sure that the passedTicks stay between the m_CurrentClip.Duration bounds (fmod)
		float passedTicks = fmod(gameContext.pGameTime->GetElapsed() * m_AnimationSpeed * m_CurrentClip.TicksPerSecond, clipDuration);

		//2. 
		if (m_Reversed == true)
		{
			//IF m_Reversed is true
			//	Subtract passedTicks from m_TickCount
			m_TickCount -= passedTicks;
			//	If m_TickCount is smaller than zero, add m_CurrentClip.Duration to m_TickCount
			if (m_TickCount < 0)
			{
				m_TickCount += clipDuration;
			}
			
		}
		else
		{
			//ELSE
			//	Add passedTicks to m_TickCount
			m_TickCount += passedTicks;
			//	if m_TickCount is bigger than the clip duration, subtract the duration from m_TickCount
			if (m_TickCount > clipDuration)
			{
				m_TickCount -= clipDuration;
			}
		}

		//3.
		//Find the enclosing keys
		AnimationKey keyA, keyB;
		//Iterate all the keys of the clip and find the following keys:
		//keyA > Closest Key with Tick before/smaller than m_TickCount
		//keyB > Closest Key with Tick after/bigger than m_TickCount
		for (size_t idx{ 0 }; idx < m_CurrentClip.Keys.size(); ++idx)
		{
			if (m_TickCount < m_CurrentClip.Keys[idx].Tick)
			{
				keyB = m_CurrentClip.Keys[idx];
				keyA = m_CurrentClip.Keys[idx-1];
				break;
			}
		}
		
		//4.
		//Interpolate between keys
		//Figure out the BlendFactor (See lab document)
		float blendFactor_A = (m_TickCount - keyA.Tick) / (keyB.Tick - keyA.Tick);
		//float blendFactor_B = (keyB.Tick - m_TickCount) / (keyB.Tick - keyA.Tick);
		//Clear the m_Transforms vector
		m_Transforms.clear();
		//FOR every boneTransform in a key (So for every bone)
		for (int idx{ 0 }; idx < m_pMeshFilter->m_BoneCount; ++idx)
		{
			//	Retrieve the transform from keyA (transformA)
			//	auto transformA = ...
			XMVECTOR TranslationA{};
			XMVECTOR RotQuatA{};
			XMVECTOR ScalingA{};
			auto &keyTransforms = keyA.BoneTransforms;
			if((unsigned int)idx < keyTransforms.size())
				//XMMatrixDecompose(&ScalingA, &RotQuatA, &TranslationA, XMLoadFloat4x4(&keyA.BoneTransforms[idx]));
				XMMatrixDecompose(&ScalingA, &RotQuatA, &TranslationA, XMLoadFloat4x4(&keyTransforms[idx]));
			
			// 	Retrieve the transform from keyB (transformB)
			//	auto transformB = ...
			XMVECTOR TranslationB;
			XMVECTOR RotQuatB;
			XMVECTOR ScalingB;
			XMMatrixDecompose(&ScalingB, &RotQuatB, &TranslationB, XMLoadFloat4x4(&keyB.BoneTransforms[idx]));
			//	Decompose both transforms
			//	Lerp between all the transformations (Position, Scale, Rotation)
			XMVECTOR finalTranslation = XMVectorLerp(TranslationA, TranslationB, blendFactor_A);
			XMVECTOR finalRotQuat = XMQuaternionSlerp(RotQuatA, RotQuatB, blendFactor_A);
			XMVECTOR finalScaling = XMVectorLerp(ScalingA, ScalingB, blendFactor_A);
			//	Compose a transformation matrix with the lerp-results
			XMMATRIX finalTransform = XMMatrixAffineTransformation(finalScaling, XMVECTOR{}, finalRotQuat, finalTranslation);
			
			//	Add the resulting matrix to the m_Transforms vector
			XMFLOAT4X4 finalTransformFloat;
			XMStoreFloat4x4(&finalTransformFloat, finalTransform);
			m_Transforms.push_back(finalTransformFloat);
			//m_Transforms.push_back(keyB.BoneTransforms[idx]);
		}

	}
}
