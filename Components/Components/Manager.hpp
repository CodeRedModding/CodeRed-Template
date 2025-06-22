#pragma once
#include "../Component.hpp"
#include "../../Modules/Includes.hpp"
#include "Variables.hpp"

namespace CodeRed
{
	static constexpr uint32_t QUEUE_TICK_MULTIPLIER = 60; // How many times per second the "ManagerComponent::OnTick" function is called.
	static constexpr float QUEUE_TICK_RATE = 1.0f; // What value the internal delta tick that the "ManagerQueue::m_delay" variable is incremented by.

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

	enum QueueFlags : uint32_t
	{
		QUEUE_None =			0UL,
		QUEUE_Completed =		1UL << 0UL, // If the queue has been fully executed and processed or not.
		QUEUE_AsyncDelay =		1UL << 1UL, // If the queue is async and its execution should be delayed by a certain amount of time.
		QUEUE_AsyncCompleted =	1UL << 2UL, // If the queues async delay has finished and can be executed.
		QUEUE_Internal =		1UL << 3UL, // If the queue should only be executed internally.
		QUEUE_SkipSave =		1UL << 4UL, // If the queue should skip saving its value if it's a setting.
	};

	class ManagerQueue
	{
	public:
		ThreadTypes m_thread; // Thread that the original command was called from.
		std::string m_command; // The command that needs to be executed.
		std::string m_arguments; // Optional arguments to execute along with the command, can be empty if there are none.
		uint32_t m_flags; // Internal flags used to store additional attributes.
		uint32_t m_delay; // Amount of ticks to delay executing the command if it's async.
		float m_delta; // Used for async commands to tell if enough time has passed to execute.

	public:
		ManagerQueue() = delete;
		ManagerQueue(ThreadTypes threadType, const std::string& command, const std::string& arguments, bool bInternal, bool bSkipSave = false);
		ManagerQueue(ThreadTypes threadType, const std::string& command, const std::string& arguments, bool bInternal, uint32_t delay, bool bSkipSave = false);
		ManagerQueue(const ManagerQueue& managerQueue);
		~ManagerQueue();

	public:
		ThreadTypes GetThread() const;
		bool IsThreadRaced() const; // Will return false if a thread racing issue has been detected.
		bool ShouldProcess() const; // If the command can be processed by "ManagerComponent" yet or not.
		const std::string& GetCommand() const;
		const std::string& GetArguments() const;
		bool HasCommand() const;
		bool HasArguments() const;
		uint32_t GetFlags() const;
		bool HasFlags(uint32_t flags) const;
		bool IsCompleted() const;
		bool IsAsync() const;
		bool IsAsyncCompleted() const;
		bool IsInternal() const;
		bool ShouldSkipSave() const;
		uint32_t GetDelay() const;
		float GetDelta() const;

	public:
		bool OnTick(float rate = QUEUE_TICK_RATE); // Returns true if the command can be executed yet or not.
		void SetThread(ThreadTypes threadType);
		void SetCompleted(bool bCompleted);
		void SetAsync(bool bAsync);
		void SetAsyncCompleted(bool bAsyncCompleted);
		void SetInternal(bool bInternal);
		void SetSkipSave(bool bSkipSave);

	private:
		void AddFlags(uint32_t queueFlags);
		void RemoveFlags(uint32_t queueFlags);

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
		std::pair<CommandResults, std::string> InternalCommand(const std::string& command, const std::string& arguments, ThreadTypes threadType = ThreadTypes::Main, bool bSkipSave = false);
		std::pair<CommandResults, std::string> InternalCommand(const std::string& command, ThreadTypes threadType = ThreadTypes::Main);
		void ConsoleCommand(const std::string& command, const std::string& arguments, ThreadTypes threadType = ThreadTypes::Main, bool bSkipSave = false);
		void ConsoleCommand(const std::string& command, ThreadTypes threadType = ThreadTypes::Main);
		void AsyncCommand(const std::string& command, const std::string& arguments, uint32_t delay, ThreadTypes threadType = ThreadTypes::Main);
		void AsyncCommand(const std::string& command, uint32_t delay, ThreadTypes threadType = ThreadTypes::Main);
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
		void QueueCommand(ManagerQueue managerQueue); // Purposefully not a const reference to create a copy.
		void ProcessCommand(const ManagerQueue& managerQueue);
		std::pair<CommandResults, std::string> ProcessCommandInternal(const ManagerQueue& managerQueue);
	};

	extern class ManagerComponent Manager;
}