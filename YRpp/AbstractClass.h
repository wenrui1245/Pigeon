#pragma once

#include <Interfaces.h>
#include <GeneralDefinitions.h>
#include <GeneralStructures.h>
#include <ArrayClasses.h>
#include <IndexClass.h>
#include <GameStrings.h>

//forward declarations
class TechnoClass;
class HouseClass;
class CRCEngine;

struct StorageClass
{
	float GetAmount(int index) const
		{ JMP_THIS(0x6C9680); }

	float GetTotalAmount() const
		{ JMP_THIS(0x6C9650); }

	float AddAmount(float amount, int index)
		{ JMP_THIS(0x6C9690); }

	float RemoveAmount(float amount, int index)
		{ JMP_THIS(0x6C96B0); }

	int GetTotalValue() const
		{ JMP_THIS(0x6C9600); }

	float Tiberium1;
	float Tiberium2;
	float Tiberium3;
	float Tiberium4;
};
//---

//The AbstractClass is the base class of all game objects.
class NOVTABLE AbstractClass : public IPersistStream, public IRTTITypeInfo, public INoticeSink, public INoticeSource
{
public:
	static const AbstractType AbsID = AbstractType::Abstract;

	static constexpr constant_ptr<DynamicVectorClass<AbstractClass*>, 0xB0F720u> const Array{};
	static constexpr reference<IndexClass<int, int>, 0xB0E840u> const TargetIndex{};

	//static
	const char* GetClassName() const
	{
		return AbstractClass::GetClassName(this->WhatAmI());
	}

	static const char* GetClassName(AbstractType abs)
	{
		const size_t TypeCount = 74;
		const auto Types = reinterpret_cast<NamedValue(*)[TypeCount]>(0x816EE0);

		for(const auto& Type : *Types) {
			if(static_cast<AbstractType>(Type.Value) == abs) {
				return Type.Name;
			}
		}

		return nullptr;
	}

	//IUnknown
	virtual HRESULT __stdcall QueryInterface(REFIID iid, void** ppvObject) R0;
	virtual ULONG __stdcall AddRef() R0;
	virtual ULONG __stdcall Release() R0;

	//IPersist
	virtual HRESULT __stdcall GetClassID(CLSID* pClassID) = 0;

	//IPersistStream
	virtual HRESULT __stdcall IsDirty() R0;
	virtual HRESULT __stdcall Load(IStream* pStm) = 0;
	virtual HRESULT __stdcall Save(IStream* pStm, BOOL fClearDirty) = 0;

	virtual HRESULT __stdcall GetSizeMax(ULARGE_INTEGER* pcbSize) R0;

	//IRTTITypeInfo
	virtual AbstractType __stdcall What_Am_I() const RT(AbstractType);
	virtual int __stdcall Fetch_ID() const R0;
	virtual void __stdcall Create_ID() RX;

	//INoticeSink
	virtual bool __stdcall INoticeSink_Unknown(DWORD dwUnknown) R0;

	//INoticeSource
	virtual void __stdcall INoticeSource_Unknown() RX;

	//Destructor
	virtual ~AbstractClass() RX;

	//AbstractClass
	virtual void Init() RX;
	virtual void PointerExpired(AbstractClass* pAbstract, bool removed) RX;
	virtual AbstractType WhatAmI() const = 0;
	virtual int Size() const = 0;
	virtual void ComputeCRC(CRCEngine& crc) const RX;
	virtual int GetOwningHouseIndex() const R0;
	virtual HouseClass* GetOwningHouse() const R0;
	virtual int GetArrayIndex() const R0;
	virtual bool IsDead() const R0;
	virtual CoordStruct* GetCoords(CoordStruct* pCrd) const R0;
	virtual CoordStruct* GetDestination(CoordStruct* pCrd, TechnoClass* pDocker = nullptr) const R0; // where this is moving, or a building's dock for a techno. iow, a rendez-vous point
	virtual bool IsOnFloor() const R0;
	virtual bool IsInAir() const R0;
	virtual CoordStruct* GetCenterCoords(CoordStruct* pCrd) const R0;
	virtual void Update() RX;

	//non-virtual
	static void __fastcall AnnounceExpiredPointer(AbstractClass* pAbstract, bool removed = true)
		{ JMP_THIS(0x7258D0); }

	static void __fastcall RemoveAllInactive() JMP_STD(0x725C70);

	void AnnounceExpiredPointer(bool removed = true) {
		AnnounceExpiredPointer(this, removed);
	}

	CoordStruct GetCoords() const {
		CoordStruct ret;
		this->GetCoords(&ret);
		return ret;
	}

	CoordStruct GetDestination(TechnoClass* pDocker = nullptr) const {
		CoordStruct ret;
		this->GetDestination(&ret, pDocker);
		return ret;
	}

	CoordStruct GetCenterCoords() const {
		CoordStruct ret;
		this->GetCenterCoords(&ret);
		return ret;
	}

	DirStruct* GetTargetDirection(DirStruct* pDir, AbstractClass* pTarget) const
		{ JMP_THIS(0x5F3DB0); }

	DirStruct GetTargetDirection(AbstractClass* pTarget) const
	{
		DirStruct ret;
		this->GetTargetDirection(&ret, pTarget);
		return ret;
	}

	//Operators
	bool operator < (const AbstractClass &rhs) const {
		return this->UniqueID < rhs.UniqueID;
	}

	//Constructor
	AbstractClass() noexcept
		: AbstractClass(noinit_t())
	{ JMP_THIS(0x410170); }

protected:
	explicit __forceinline AbstractClass(noinit_t) noexcept
	{ }

	//===========================================================================
	//===== Properties ==========================================================
	//===========================================================================

public:

	DWORD UniqueID; // generated by IRTTIInfo::Create_ID through an amazingly simple sequence of return ++ScenarioClass::Instance->UniqueID;
	AbstractFlags AbstractFlags;	// flags, see AbstractFlags enum in GeneralDefinitions.
	DWORD unknown_18;
	LONG RefCount;
	bool Dirty;		// for IPersistStream.
	PROTECTED_PROPERTY(BYTE, padding_21[0x3]);
};

template<typename T>
concept HasAbsVTable = std::is_base_of_v<AbstractClass,T> && requires {
	{ T::AbsVTable }-> std::convertible_to<uintptr_t>;
};
