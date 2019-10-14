#pragma once
#include "../Base/GeneralStructs.h"

//Forward Declarations
class GameObject;
class SceneManager;
class CameraComponent;
class PhysxProxy;
class PostProcessingMaterial;

class GameScene
{
public:
	GameScene(wstring sceneName);
	virtual ~GameScene(void);

	void AddChild(GameObject* obj);
	void PendToAddChild(GameObject* obj);
	void PendToRemoveChild(GameObject* pToKillObject);
	void RemoveChild(GameObject* obj, bool deleteObject = true);
	const GameContext& GetGameContext() const { return m_GameContext; }

	PhysxProxy * GetPhysxProxy() const { return m_pPhysxProxy; }
	void SetActiveCamera(CameraComponent* pCameraComponent);
	const std::wstring Getname() const;

	void AddPostProcessingMaterial(UINT materialId_PP);
	void RemovePostProcessingMaterial(UINT materialId_PP);

protected:

	virtual void Initialize(const GameContext& gameContext) = 0;
	virtual void Update(const GameContext& gameContext) = 0;
	virtual void Draw(const GameContext& gameContext) = 0;
	virtual void SceneActivated(){}
	virtual void SceneDeactivated(){}

	vector<UINT> m_PostProcessingMaterialIds;
	bool m_IsPaused;
private:

	friend class SceneManager;

	void RootInitialize(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	void RootUpdate();
	void RootDraw();
	void RootSceneActivated();
	void RootSceneDeactivated();
	void RootWindowStateChanged(int state, bool active);

	void KillPendingObjects();
	void AddPendingObjects();

	vector<GameObject*> m_pChildren;
	GameContext m_GameContext;
	bool m_IsInitialized;
	wstring m_SceneName;
	CameraComponent *m_pDefaultCamera, *m_pActiveCamera;
	PhysxProxy* m_pPhysxProxy;

	std::vector<GameObject*> m_ToKillObjects;
	std::vector<GameObject*> m_ToAddObjects;

private:
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	GameScene( const GameScene &obj);
	GameScene& operator=( const GameScene& obj);
};
