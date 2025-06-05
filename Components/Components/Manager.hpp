#pragma once
#include "../Component.hpp"
#include "../../Modules/Includes.hpp"
#include "Variables.hpp"

namespace CodeRed
{
	enum class CommandResults : uint8_t
	{
		None,
		Queued,
		Unrecognized,
		EmptyArguments,
		InvalidArguments,
		ModifySetting,
		PrintSetting
	};

	class ManagerQueue
	{
	public:
		ThreadTypes Thread;
		std::string Command;
		std::string Arguments;
		uint64_t Delay;
		uint64_t Delta;
		bool SkipSave;
		bool Internal;
		bool Completed;

	public:
		ManagerQueue() = delete;
		ManagerQueue(const std::string& command, const std::string& arguments, bool bInternal, bool bSkipSave = false);
		ManagerQueue(const std::string& command, const std::string& arguments, bool bInternal, uint64_t delay, uint32_t multiplier = 60, bool bSkipSave = false);
		ManagerQueue(const ManagerQueue& managerQueue);
		~ManagerQueue();

	public:
		bool HasArguments() const;
		bool IsAsync() const;
		bool OnTick();

	public:
		ManagerQueue& operator=(const ManagerQueue& managerQueue);
	};

	// Manages variables, commands, settings, and modules.
	class ManagerComponent : public Component
	{
	private:
		std::map<std::string, std::shared_ptr<Module>> m_modules;
		std::vector<ManagerQueue> m_threadQueue;
		std::vector<ManagerQueue> m_queue;
		std::atomic<bool> m_queueLocked; // Used for preventing a thread racing issue when calling commands from different threads.

	public:
		std::shared_ptr<PlaceholderModule> PlaceholderMod;

	public:
		ManagerComponent();
		~ManagerComponent() override;

	public:
		void OnCreate() override;
		void OnDestroy() override;
		bool Initialize() override;

	public:
		void OnTick(); // Checks the queue to see if there are any commands or settings that need to be processed.
		void OnCanvasDraw(class UCanvas* unrealCanvas); // Calls all canvas drawing functions in each module, called 60 times every second on the games main thread.

	public:
		std::pair<CommandResults, std::string> InternalCommand(const std::string& command, const std::string& arguments, ThreadTypes thread = ThreadTypes::Main, bool bSkipSave = false);
		std::pair<CommandResults, std::string> InternalCommand(const std::string& command, ThreadTypes thread = ThreadTypes::Main);
		void ConsoleCommand(const std::string& command, const std::string& arguments, ThreadTypes thread = ThreadTypes::Main, bool bSkipSave = false);
		void ConsoleCommand(const std::string& command, ThreadTypes thread = ThreadTypes::Main);
		void AsyncCommand(const std::string& command, const std::string& arguments, uint64_t delay, ThreadTypes thread = ThreadTypes::Main);
		void AsyncCommand(const std::string& command, uint64_t delay, ThreadTypes thread = ThreadTypes::Main);
		void UnrealCommand(std::string unrealCommand, bool bPrintToConsole = true);

	public:
		template <typename T> std::shared_ptr<T> CreateModule(Module* mod, std::shared_ptr<T>& moduleToBind);
		template <typename T> std::shared_ptr<T> GetModule(const std::string& moduleName)
		{
			if (m_modules.contains(moduleName))
			{
				return std::static_pointer_cast<T>(m_modules[moduleName]);
			}

			return nullptr;
		}
		void UpdateAllSettings();

	private:
		void QueueCommand(const ManagerQueue& managerQueue, ThreadTypes thread);
		void ProcessCommand(const ManagerQueue& managerQueue, ThreadTypes thread = ThreadTypes::Main);
		std::pair<CommandResults, std::string> ProcessCommandInternal(const ManagerQueue& managerQueue, ThreadTypes thread = ThreadTypes::Main);
	};

	extern class ManagerComponent Manager;
}