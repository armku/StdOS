#ifndef __DevMgmt_H__
#define __DevMgmt_H__

#include "..\Core\List.h"
#include "Kernel\Sys.h"
#include "Device.h"

#include "Device\Flash.h"
#include "TinyNet\TinyConfig.h"
#include "Message\BinaryPair.h"

#include "TokenClient.h"
#include "DeviceMessage.h"
#include "Security\Crc.h"

// DevicesManagement 服务于客户端/云端   所以内部 Port 限定为TokenClient
class DevicesManagement
{
public:
	~DevicesManagement();

	byte LocalId;
	int Length() { return DevArr.Count(); }

	Device* FindDev(byte id)const;
	Device* FindDev(const Buffer& hardid) const;

	void ClearDev();
	void ShowDev();

private:
	// 不允许外部调用  避免重复LoadDev()
	int LoadDev();
	bool SetFlashCfg(uint addr, uint size);

	// 外部操作使用 DeviceRequest
	void SaveDev();
	bool DeleteDev(byte id);
	int	PushDev(Device* dv);

public:
	// 设备列表
	IList	DevArr;
	TokenClient * Port = nullptr;
	// 发送时刻再绑定？！ 如果绑定失败报错？
	void SetTokenClient(TokenClient *port);

	bool DeviceProcess(DeviceAtions act,const Pair& args, Stream& result);
	void SendDevicesIDs();
	// 设备状态变更上报 由TinyServer这种对象调用
	void DeviceRequest(DeviceAtions act, const Device* dv = nullptr);
	void DeviceRequest(DeviceAtions act, byte id);

private:
	// 把ids写入到ms
	int WriteIDs(Stream & ms);
	// 获取设备信息到 MemoryStream (名值对形式)
	bool GetDevInfo(byte id, MemoryStream &ms);
	bool GetDevInfo(Device *dv, MemoryStream &ms);
	// 发送单个设备信息  只在维护设备状态时使用
	bool SendDevices(DeviceAtions act, const Device* dv = nullptr);

public:
	// 维护设备状态
	void MaintainState();

	typedef void(*DevPrsCallback)(DeviceAtions act, const Device* dv, void * param);
	// 注册到使用 TokenMsg Delete 设备的时候
	void Register(DevPrsCallback func, void* param) { _DevProcess = func; _ClbkParam = param; };

private:
	const Config GetStore(Flash &flash);
	uint _Addr = 0;		// 固化到flash的地址
	uint _FlashSize = 0;

	DevPrsCallback  _DevProcess = nullptr;
	void * _ClbkParam = nullptr;

public:
	// 使用默认最后4K空间设置 addr = 0x8000000 + (Sys.FlashSize << 10) - (4 << 10);
	static DevicesManagement* CreateDevMgmt(uint addr = 0, uint size = 0);

	// 全局只允许一个设备管理器  Invoke 也就使用这个
	static DevicesManagement * Current;
	static bool InvokeFindIDs(void * param, const Pair& args, Stream& result);
	static bool InvokeFindAll(void * param, const Pair& args, Stream& result);
	static bool InvokeSet(void * param, const Pair& args, Stream& result);
	static bool InvokeDelete(void * param, const Pair& args, Stream& result);
};


#endif // !__DevMgmt_H__
