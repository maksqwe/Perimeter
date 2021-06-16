// TODO: change encoding to utf-8

#ifndef __CONDITIONS_H__
#define __CONDITIONS_H__

#include "../TriggerEditor/TriggerExport.h"

struct ConditionOneTime : Condition // --------------------
{
	bool check(AIPlayer& aiPlayer) { return satisfiedTimer_(); }
	void setSatisfied(int time = 3000) { satisfiedTimer_.start(time); }
	void clear() { satisfiedTimer_.stop(); }

private:
	DurationTimer satisfiedTimer_;
};

struct ConditionIsPlayerAI : Condition // �� �� �����
{
	bool check(AIPlayer& aiPlayer);
};

struct ConditionCheckBelligerent : Condition // �������� ������� �������
{
	enum Belligerent {
		EXODUS, // ���������
		HARKBACKHOOD, // �����������
		EMPIRE // �������
	};

	EnumWrapper<Belligerent> belligerent; 

	ConditionCheckBelligerent() {
		belligerent = EXODUS; 
	}

	bool check(AIPlayer& aiPlayer);

	template<class Archive>	
	void serialize(Archive& ar) {
		Condition::serialize(ar);
		ar & TRANSLATE_OBJECT(belligerent, "������� �������");
	}
};

//---------------------------------------
struct ConditionCreateObject : Condition // ������ ������
{
	EnumWrapper<terUnitAttributeID> object; 
	int counter; 
	EnumWrapper<AIPlayerType> playerType; 

	ConditionCreateObject() {
		object = UNIT_ATTRIBUTE_FRAME; 
		counter = 1; 
		playerType = AI_PLAYER_TYPE_ME; 
		created_ = 0;
	}

	bool check(AIPlayer& aiPlayer) { return created_ >= counter; }
	void checkEvent(AIPlayer& aiPlayer, const Event& event);

	template<class Archive>	
	void serialize(Archive& ar) {
		Condition::serialize(ar);
		ar & TRANSLATE_OBJECT(object, "������");
		ar & TRANSLATE_OBJECT(counter, "����������");
		ar & TRANSLATE_OBJECT(playerType, "�������� �������");
		created_ = 0;
	}

protected:
	int created_;
};

struct ConditionKillObject : Condition // ������ ���������
{
	EnumWrapper<terUnitAttributeID> object; 
	int counter; 
	EnumWrapper<AIPlayerType> playerType;

	ConditionKillObject() {
		object = UNIT_ATTRIBUTE_FRAME; 
		counter = 1; 
		playerType = AI_PLAYER_TYPE_ME;
		killed_ = 0;
	}

	bool check(AIPlayer& aiPlayer) { return killed_ >= counter; }
	void checkEvent(AIPlayer& aiPlayer, const Event& event);

	template<class Archive>	
	void serialize(Archive& ar){
		Condition::serialize(ar);
		ar & TRANSLATE_OBJECT(object, "������");
		ar & TRANSLATE_OBJECT(counter, "����������");
		ar & TRANSLATE_OBJECT(playerType, "�������� �������");
		killed_ = 0;
	}

private:
	int killed_;
};

struct ConditionObjectExists : Condition // ������ ����������
{
	EnumWrapper<terUnitAttributeID> object; 
	int counter; 
	EnumWrapper<AIPlayerType> playerType;
	bool constructedAndConstructing; 

	ConditionObjectExists() {
		object = UNIT_ATTRIBUTE_FRAME; 
		counter = 1; 
		playerType = AI_PLAYER_TYPE_ME;
		constructedAndConstructing = true; 
	}

	bool check(AIPlayer& aiPlayer);

	template<class Archive>	
	void serialize(Archive& ar) {
		Condition::serialize(ar);
		ar & TRANSLATE_OBJECT(object, "������");
		ar & TRANSLATE_OBJECT(counter, "����������");
		ar & TRANSLATE_OBJECT(playerType, "�������� �������");
		ar & TRANSLATE_OBJECT(constructedAndConstructing, "����������� � ��� ����������");
	}
};	

struct ConditionCaptureBuilding : ConditionOneTime // ������ ������
{
	EnumWrapper<terUnitAttributeID> object; 
	EnumWrapper<AIPlayerType> playerType;

	ConditionCaptureBuilding() {
		object = UNIT_ATTRIBUTE_COLLECTOR; 
		playerType = AI_PLAYER_TYPE_ME;
	}

	void checkEvent(AIPlayer& aiPlayer, const Event& event);

	template<class Archive>	
	void serialize(Archive& ar) {
		ConditionOneTime::serialize(ar);
		ar & TRANSLATE_OBJECT(object, "������");
		ar & TRANSLATE_OBJECT(playerType, "����������� �����");
	}
};

//---------------------------------------
enum TeleportationType
{
	TELEPORTATION_TYPE_ALPHA, // ������������ � ������� �����
	TELEPORTATION_TYPE_OMEGA // ������������ � ������� �����
};
struct ConditionTeleportation : ConditionOneTime // ��������� ������������
{
	EnumWrapper<TeleportationType> teleportationType; 
	EnumWrapper<AIPlayerType> playerType; 

	ConditionTeleportation() {
		teleportationType = TELEPORTATION_TYPE_ALPHA; 
		playerType = AI_PLAYER_TYPE_ME; 
	}

	void checkEvent(AIPlayer& aiPlayer, const Event& event);

	template<class Archive>	
	void serialize(Archive& ar) {
		ConditionOneTime::serialize(ar);
		ar & TRANSLATE_OBJECT(teleportationType, "��� ���������");
		ar & TRANSLATE_OBJECT(playerType, "�����");
	}
};	

//---------------------------------------
struct ConditionEnegyLevelLowerReserve : Condition // ������� ������� ���� �������
{
	float energyReserve; 

	ConditionEnegyLevelLowerReserve() {
		energyReserve = 300; 
	}

	bool check(AIPlayer& aiPlayer);

	template<class Archive>	
	void serialize(Archive& ar) {
		Condition::serialize(ar);
		ar & TRANSLATE_OBJECT(energyReserve, "������ �������");
	}
};

struct ConditionEnegyLevelUpperReserve : Condition // ������� ������� ���� �������
{
	float energyReserve; 

	ConditionEnegyLevelUpperReserve() {
		energyReserve = 300; 
	}

	bool check(AIPlayer& aiPlayer);

	template<class Archive>	
	void serialize(Archive& ar) {
		Condition::serialize(ar);
		ar & TRANSLATE_OBJECT(energyReserve, "������ �������");
	}
};

struct ConditionEnegyLevelBelowMaximum : Condition // ������� ������� ���� ���������
{
	float delta; 

	ConditionEnegyLevelBelowMaximum() {
		delta = 50; 
		accumulatedMax_ = 0;
	}

	bool check(AIPlayer& aiPlayer);

	template<class Archive>	
	void serialize(Archive& ar) {
		Condition::serialize(ar);
		ar & TRANSLATE_OBJECT(delta, "�����������: ������� < ������������ - �����������");
		accumulatedMax_ = 0;
	}

private:
	float accumulatedMax_;
};

struct ConditionOutOfEnergyCapacity : Condition // ������� ������� ������ �������� �������
{
	float chargingPercent; 

	ConditionOutOfEnergyCapacity() {
		chargingPercent = 99; 
	}

	bool check(AIPlayer& aiPlayer);

	template<class Archive>	
	void serialize(Archive& ar) {
		Condition::serialize(ar);
		ar & TRANSLATE_OBJECT(chargingPercent, "������� �������");
	}
};

struct ConditionNumberOfBuildingByCoresCapacity : Condition // ������_1*����������� < ������_2
{
	EnumWrapper<terUnitAttributeID> building; 
	float factor; 
	EnumWrapper<CompareOperator> compareOp; 
	EnumWrapper<terUnitAttributeID> building2; 
	EnumWrapper<AIPlayerType> playerType; 

	ConditionNumberOfBuildingByCoresCapacity() {
		building = UNIT_ATTRIBUTE_LASER_CANNON; 
		factor = 1; 
		compareOp = COMPARE_LESS; 
		building2 = UNIT_ATTRIBUTE_CORE; 
		playerType = AI_PLAYER_TYPE_ME; 
	}

	bool check(AIPlayer& aiPlayer);

	template<class Archive>	
	void serialize(Archive& ar) {
		Condition::serialize(ar);
		ar & TRANSLATE_OBJECT(building, "��� ������_1");
		ar & TRANSLATE_OBJECT(factor, "�����������");
		ar & TRANSLATE_OBJECT(compareOp, "�������� ���������");
		ar & TRANSLATE_OBJECT(building2, "��� ������_2");
		ar & TRANSLATE_OBJECT(playerType, "�����");
	}
};


//---------------------------------------
struct ConditionUnitClassUnderAttack : ConditionOneTime // ������ �������
{
	BitVector<terUnitClassType> victimUnitClass; 
	int damagePercent; 
	BitVector<terUnitClassType> agressorUnitClass; 
	EnumWrapper<AIPlayerType> playerType; 

	ConditionUnitClassUnderAttack() {
		damagePercent = 0; 
		playerType = AI_PLAYER_TYPE_ME; 
	}

	void checkEvent(AIPlayer& aiPlayer, const Event& event);

	template<class Archive>	
	void serialize(Archive& ar) {
		ConditionOneTime::serialize(ar);
		ar & TRANSLATE_OBJECT(victimUnitClass, "��������� ����� ������");
		ar & TRANSLATE_OBJECT(damagePercent, "������� �����");
		ar & TRANSLATE_OBJECT(agressorUnitClass, "��������� ����� ������");
		ar & TRANSLATE_OBJECT(playerType, "�������� �������");
	}
};

struct ConditionUnitClassIsGoingToBeAttacked : ConditionOneTime // ������ ���������� ���������
{
	BitVector<terUnitClassType> victimUnitClass; 
	BitVector<terUnitClassType> agressorUnitClass; 

	void checkEvent(AIPlayer& aiPlayer, const Event& event);

	template<class Archive>	
	void serialize(Archive& ar) {
		ConditionOneTime::serialize(ar);
		ar & TRANSLATE_OBJECT(victimUnitClass, "��������� ����� ������");
		ar & TRANSLATE_OBJECT(agressorUnitClass, "��������� ����� ������");
	}
};

struct ConditionSquadGoingToAttack : Condition // ����� ���������� ���������
{
	EnumWrapper<ChooseSquadID> chooseSquadID; 

	ConditionSquadGoingToAttack() {
		chooseSquadID = CHOOSE_SQUAD_1; 
	}

	bool check(AIPlayer& aiPlayer);

	template<class Archive>	
	void serialize(Archive& ar) {
		Condition::serialize(ar);
		ar & TRANSLATE_OBJECT(chooseSquadID, "�����");
	}
};

//---------------------------------------
struct ConditionFrameState : Condition // ��������� ������
{
	enum State // ��������� ������
	{
		AI_FRAME_STATE_EXIST, // ����������
		AI_FRAME_STATE_INSTALLED, // �������������
		AI_FRAME_STATE_INSTALLING, // �������������� � ������ ������
		AI_FRAME_STATE_POWERED, // ���������
		AI_FRAME_STATE_MOVING, // ���������
		AI_FRAME_STATE_TELEPORTATION_ENABLED, // ������������ ���������
		AI_FRAME_STATE_TELEPORTATION_STARTED, // ������������ ��������
		AI_FRAME_STATE_SPIRAL_CHARGING // ������� �������� �� X % � �����
	};
	EnumWrapper<State> state; 
	EnumWrapper<AIPlayerType> playerType; 
	int spiralChargingPercent; 

	ConditionFrameState() {
		state = AI_FRAME_STATE_INSTALLED; 
		playerType = AI_PLAYER_TYPE_ME; 
		spiralChargingPercent = 100; 
	}

	bool check(AIPlayer& aiPlayer);

	template<class Archive>	
	void serialize(Archive& ar) {
		Condition::serialize(ar);
		ar & TRANSLATE_OBJECT(state, "������������ ���������");
		ar & TRANSLATE_OBJECT(playerType, "�����");
		ar & TRANSLATE_OBJECT(spiralChargingPercent, "������� ������� �������");
	}
};

struct ConditionCorridorOmegaUpgraded : Condition // ������� ����� ������������
{
	bool check(AIPlayer& aiPlayer);
};

//---------------------------------------
struct ConditionSquadSufficientUnits : Condition // C���� ������� �� ������ � ��������� ����������
{
	EnumWrapper<AIPlayerType> playerType;
	EnumWrapper<ChooseSquadID> chooseSquadID; 
	EnumWrapper<terUnitAttributeID> unitType; 
	EnumWrapper<CompareOperator> compareOperator; 
	int unitsNumber;
	int soldiers; 
	int officers; 
	int technics; 

	ConditionSquadSufficientUnits() {
		playerType = AI_PLAYER_TYPE_ME;
		chooseSquadID = CHOOSE_SQUAD_1;
		unitType = UNIT_ATTRIBUTE_NONE;
		compareOperator = COMPARE_EQ;
		unitsNumber = 0;
		soldiers = 0;
		officers = 0;
		technics = 0;
	}

	bool check(AIPlayer& aiPlayer);

	template<class Archive>	
	void serialize(Archive& ar) {
		Condition::serialize(ar);
		ar & TRANSLATE_OBJECT(playerType, "��������");
		ar & TRANSLATE_OBJECT(chooseSquadID, "�����");
		ar & TRANSLATE_OBJECT(unitType, "��� ������ ('�����' - �������)");
		ar & TRANSLATE_OBJECT(compareOperator, "�������� ���������");
		ar & TRANSLATE_OBJECT(unitsNumber, "�����������");
		ar & TRANSLATE_OBJECT(soldiers, "�������� ��� �������");
		ar & TRANSLATE_OBJECT(officers, "�������� ��� �������");
		ar & TRANSLATE_OBJECT(technics, "�������� ��� �������");
	}
};

struct ConditionMutationEnabled : Condition // ������� ���������
{
	EnumWrapper<terUnitAttributeID> unitType; 

	ConditionMutationEnabled() {
		unitType = UNIT_ATTRIBUTE_NONE; 
	}

	bool check(AIPlayer& aiPlayer);

	template<class Archive>	
	void serialize(Archive& ar) {
		Condition::serialize(ar);
		ar & TRANSLATE_OBJECT(unitType, "��� ������");
	}
};

struct ConditionBuildingNearBuilding : Condition // ���������� �� ���� ������1 �� ������ ������ ������2 ������ �������������
{
	float distance; 
	EnumWrapper<AIPlayerType> playerType1; 
	EnumWrapper<AIPlayerType> playerType2; 

	ConditionBuildingNearBuilding() {
		distance = 500;
		playerType1 = AI_PLAYER_TYPE_ME;
		playerType2 = AI_PLAYER_TYPE_ENEMY;
		index_ = 0;
	}

	bool check(AIPlayer& aiPlayer);

	template<class Archive>	
	void serialize(Archive& ar) {
		Condition::serialize(ar);
		ar & TRANSLATE_OBJECT(distance, "����������");
		ar & TRANSLATE_OBJECT(playerType1, "�����1");
		ar & TRANSLATE_OBJECT(playerType2, "�����2");
		index_ = 0;
	}

private:
	int index_;
};

enum PlayerState // ��������� ������ 
{
	PLAYER_STATE_UNABLE_TO_PLACE_BUILDING, // �� ���� ���������� ������
	PLAYER_STATE_UNABLE_TO_PLACE_CORE // �� ���� ���������� ����
};

struct ConditionPlayerState : Condition // �������� ��������� ������
{
	EnumWrapper<PlayerState> playerState; 

	ConditionPlayerState() {
		playerState = PLAYER_STATE_UNABLE_TO_PLACE_BUILDING; 
		active_ = false;
	}

	bool check(AIPlayer& aiPlayer) { return active_; }
	void checkEvent(AIPlayer& aiPlayer, const Event& event);

	template<class Archive>	
	void serialize(Archive& ar) {
		Condition::serialize(ar);
		ar & TRANSLATE_OBJECT(playerState, "������������ ���������");
		active_ = false;
	}

private:
	bool active_;
};

struct ConditionIsFieldOn : Condition // ���� ��������
{
	bool check(AIPlayer& aiPlayer);
};

//---------------------------------------
struct ConditionObjectByLabelExists : Condition // ������ �� ����� ����������
{
	CustomString label; 

	ConditionObjectByLabelExists() :
	label(editLabelDialog)
	{}

	bool check(AIPlayer& aiPlayer);

	template<class Archive>	
	void serialize(Archive& ar) {
		Condition::serialize(ar);
		ar & TRANSLATE_OBJECT(label, "����� �������");
	}
};	

struct ConditionKillObjectByLabel : ConditionOneTime // ������ �� ����� ���������
{
	CustomString label; 

	ConditionKillObjectByLabel() :
	label(editLabelDialog)
	{}

	void checkEvent(AIPlayer& aiPlayer, const Event& event);

	template<class Archive>	
	void serialize(Archive& ar) { 
		ConditionOneTime::serialize(ar);
		ar & TRANSLATE_OBJECT(label, "����� �������");
	}
};

struct ConditionObjectNearObjectByLabel : Condition // ����� ������� �� ����� ��������� ������ ���������� ����
{
	CustomString label; 
	EnumWrapper<terUnitAttributeID> object; 
	bool objectConstructed; 
	EnumWrapper<AIPlayerType> playerType; 
	float distance; 

	ConditionObjectNearObjectByLabel() :
	label(editLabelDialog)
	{
		object = UNIT_ATTRIBUTE_CORE; 
		objectConstructed = false; 
		playerType = AI_PLAYER_TYPE_ME; 
		distance = 100; 
	}

	bool check(AIPlayer& aiPlayer);

	template<class Archive>	
	void serialize(Archive& ar) {
		Condition::serialize(ar);
		ar & TRANSLATE_OBJECT(label, "����� �������");
		ar & TRANSLATE_OBJECT(object, "��������� ������");
		ar & TRANSLATE_OBJECT(objectConstructed, "������ ���� ������ ��������");
		ar & TRANSLATE_OBJECT(playerType, "�������� �������");
		ar & TRANSLATE_OBJECT(distance, "������������ ����������");
	}
};

struct ConditionWeaponIsFiring : Condition // ���������� ��������
{
	EnumWrapper<terUnitAttributeID> gun; 

	ConditionWeaponIsFiring() {
		gun = UNIT_ATTRIBUTE_GUN_SCUM_DISRUPTOR; 
	}

	bool check(AIPlayer& aiPlayer);

	template<class Archive>	
	void serialize(Archive& ar) {
		Condition::serialize(ar);
		ar & TRANSLATE_OBJECT(gun, "����������");
	}
};

struct ConditionTimeMatched : ConditionOneTime // �������� ������� ������, ��� �������
{
	int time; 
	
	ConditionTimeMatched() {
		time = 60; 
	}

	void checkEvent(AIPlayer& aiPlayer, const Event& event);

	template<class Archive>	
	void serialize(Archive& ar) {
		ConditionOneTime::serialize(ar);
		ar & TRANSLATE_OBJECT(time, "�����, �������");
	}
};

struct ConditionMouseClick : ConditionOneTime // ���� ����
{
	void checkEvent(AIPlayer& aiPlayer, const Event& event);
};

struct ConditionClickOnButton : Condition // ���� �� ������
{
	EnumWrapper<ShellControlID> controlID; 
	int counter; 

	ConditionClickOnButton() {
		controlID = SQSH_STATIC_ID; 
		counter = 1; 
		counter_ = 0;
	}

	bool check(AIPlayer& aiPlayer) { return counter_ >= counter; }
	void checkEvent(AIPlayer& aiPlayer, const Event& event);

	template<class Archive>	
	void serialize(Archive& ar) {
		Condition::serialize(ar);
		ar & TRANSLATE_OBJECT(controlID, "������������� ������");
		ar & TRANSLATE_OBJECT(counter, "���������� ������");
		counter_ = 0;
	}

private:
	int counter_;
};

struct ConditionToolzerSelectedNearObjectByLabel : Condition // ����������� ����� ������� �� ����� �������� ��� ������������
{
	CustomString label;
	int radius; 

	ConditionToolzerSelectedNearObjectByLabel() :
	label(editLabelDialog)
	{
		radius = 100; 
	}

	bool check(AIPlayer& aiPlayer);

	template<class Archive>	
	void serialize(Archive& ar) {
		Condition::serialize(ar);
		ar & TRANSLATE_OBJECT(label, "����� �������");
		ar & TRANSLATE_OBJECT(radius, "������");
	}
};	

struct ConditionTerrainLeveledNearObjectByLabel : Condition // ����������� ����� ������� �� ����� ���������
{
	CustomString label; 
	int radius;

	ConditionTerrainLeveledNearObjectByLabel() :
	label(editLabelDialog)
	{
		radius = 100;
	}
	
	bool check(AIPlayer& aiPlayer);

	template<class Archive>	
	void serialize(Archive& ar) {
		Condition::serialize(ar);
		ar & TRANSLATE_OBJECT(label, "����� �������");
		ar & TRANSLATE_OBJECT(radius, "������");
	}
};	

struct ConditionSetSquadWayPoint : Condition // ������ ������ ����������
{
	bool check(AIPlayer& aiPlayer);
};	

struct ConditionActivateSpot : ConditionOneTime // ������������� ����
{
	enum Type {
		FILTH = 1, // �������
		GEO = 2 // ���
	};
	BitVector<Type> type; 

	ConditionActivateSpot() {
		type = FILTH | GEO; 
	}

	void checkEvent(AIPlayer& aiPlayer, const Event& event);

	template<class Archive>	
	void serialize(Archive& ar) {
		ConditionOneTime::serialize(ar);
		ar & TRANSLATE_OBJECT(type, "��� �����");
	}
};


struct ConditionOnlyMyClan : ConditionOneTime // ������� ������ ��� ����
{
	bool check(AIPlayer& aiPlayer);
};

struct ConditionSkipCutScene : Condition // ���������� ���-�����
{
	bool check(AIPlayer& aiPlayer);
};

struct ConditionCutSceneWasSkipped : ConditionSkipCutScene // ���-����� ���� ���������
{
	int timeMax; 

	ConditionCutSceneWasSkipped() {
		timeMax = 5; 
	}

	bool check(AIPlayer& aiPlayer);

	template<class Archive>	
	void serialize(Archive& ar) {
		ConditionSkipCutScene::serialize(ar);
		ar & TRANSLATE_OBJECT(timeMax, "������������ �����");
	}
};

struct ConditionDifficultyLevel : Condition // ������� ���������
{
	EnumWrapper<Difficulty> difficulty; 

	ConditionDifficultyLevel() {
		difficulty = DIFFICULTY_HARD; 
	}

	bool check(AIPlayer& aiPlayer);

	template<class Archive>	
	void serialize(Archive& ar) {
		Condition::serialize(ar);
		ar & TRANSLATE_OBJECT(difficulty, "�������");
	}
};


#endif //__CONDITIONS_H__