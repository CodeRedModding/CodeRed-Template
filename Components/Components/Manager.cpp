#include "Manager.hpp"
#include "../Includes.hpp"

namespace CodeRed
{
	ManagerQueue::ManagerQueue(ThreadTypes threadType, const std::string& command, const std::string& arguments, bool bInternal, bool bSkipSave) :
		m_thread(threadType),
		m_command(command),
		m_arguments(arguments),
		m_flags(QueueFlags::QUEUE_None),
		m_delay(0),
		m_delta(0.0f)
	{
		SetInternal(bInternal);
		SetSkipSave(bSkipSave);
	}

	ManagerQueue::ManagerQueue(ThreadTypes threadType, const std::string& command, const std::string& arguments, bool bInternal, uint32_t delay, bool bSkipSave) :
		m_thread(threadType),
		m_command(command),
		m_arguments(arguments),
		m_flags(QueueFlags::QUEUE_None),
		m_delay(delay* QUEUE_TICK_MULTIPLIER),
		m_delta(0.0f)
	{
		SetInternal(bInternal);
		SetAsync(m_delay > 0);
		SetSkipSave(bSkipSave);
	}

	ManagerQueue::ManagerQueue(const ManagerQueue& managerQueue) :
		m_thread(managerQueue.m_thread),
		m_command(managerQueue.m_command),
		m_arguments(managerQueue.m_arguments),
		m_flags(managerQueue.m_flags),
		m_delay(managerQueue.m_delay),
		m_delta(managerQueue.m_delta)
	{

	}

	ManagerQueue::~ManagerQueue() {}

	ThreadTypes ManagerQueue::GetThread() const
	{
		return m_thread;
	}

	bool ManagerQueue::IsThreadRaced() const
	{
		return ((GetThread() != ThreadTypes::Main) && (GetThread() != ThreadTypes::Render));
	}

	bool ManagerQueue::ShouldProcess() const
	{
		return (!IsThreadRaced() && (GetThread() == ThreadTypes::Main) && (!IsAsync() || IsAsyncCompleted()));
	}

	const std::string& ManagerQueue::GetCommand() const
	{
		return m_command;
	}

	const std::string& ManagerQueue::GetArguments() const
	{
		return m_arguments;
	}

	bool ManagerQueue::HasCommand() const
	{
		return !GetCommand().empty();
	}

	bool ManagerQueue::HasArguments() const
	{
		return !GetArguments().empty();
	}

	uint32_t ManagerQueue::GetFlags() const
	{
		return m_flags;
	}

	bool ManagerQueue::HasFlags(uint32_t flags) const
	{
		return (GetFlags() & flags);
	}

	bool ManagerQueue::IsCompleted() const
	{
		return HasFlags(QueueFlags::QUEUE_Completed);
	}

	bool ManagerQueue::IsAsync() const
	{
		return HasFlags(QueueFlags::QUEUE_AsyncDelay);
	}

	bool ManagerQueue::IsAsyncCompleted() const
	{
		return HasFlags(QueueFlags::QUEUE_AsyncCompleted);
	}

	bool ManagerQueue::IsInternal() const
	{
		return HasFlags(QueueFlags::QUEUE_Internal);
	}

	bool ManagerQueue::ShouldSkipSave() const
	{
		return HasFlags(QueueFlags::QUEUE_SkipSave);
	}

	uint32_t ManagerQueue::GetDelay() const
	{
		return m_delay;
	}

	float ManagerQueue::GetDelta() const
	{
		return m_delta;
	}

	bool ManagerQueue::OnTick(float rate)
	{
		if (IsAsync() && !IsAsyncCompleted())
		{
			m_delta += rate;

			if (GetDelta() < GetDelay())
			{
				return false;
			}

			SetAsyncCompleted(true);
		}

		return true;
	}

	void ManagerQueue::SetThread(ThreadTypes threadType)
	{
		m_thread = threadType;
	}

	void ManagerQueue::SetCompleted(bool bCompleted)
	{
		if (bCompleted)
		{
			AddFlags(QueueFlags::QUEUE_Completed);
		}
		else
		{
			RemoveFlags(QueueFlags::QUEUE_Completed);
		}
	}

	void ManagerQueue::SetAsync(bool bAsync)
	{
		if (bAsync)
		{
			AddFlags(QueueFlags::QUEUE_AsyncDelay);
		}
		else
		{
			RemoveFlags(QueueFlags::QUEUE_AsyncDelay);
		}
	}

	void ManagerQueue::SetAsyncCompleted(bool bAsyncCompleted)
	{
		if (bAsyncCompleted)
		{
			AddFlags(QueueFlags::QUEUE_AsyncCompleted);
		}
		else
		{
			RemoveFlags(QueueFlags::QUEUE_AsyncCompleted);
		}
	}

	void ManagerQueue::SetInternal(bool bInternal)
	{
		if (bInternal)
		{
			AddFlags(QueueFlags::QUEUE_Internal);
		}
		else
		{
			RemoveFlags(QueueFlags::QUEUE_Internal);
		}
	}

	void ManagerQueue::SetSkipSave(bool bSkipSave)
	{
		if (bSkipSave)
		{
			AddFlags(QueueFlags::QUEUE_SkipSave);
		}
		else
		{
			RemoveFlags(QueueFlags::QUEUE_SkipSave);
		}
	}

	void ManagerQueue::AddFlags(uint32_t queueFlags)
	{
		m_flags |= queueFlags;
	}

	void ManagerQueue::RemoveFlags(uint32_t queueFlags)
	{
		m_flags &= ~queueFlags;
	}

	ManagerQueue& ManagerQueue::operator=(const ManagerQueue& managerQueue)
	{
		m_thread = managerQueue.m_thread;
		m_command = managerQueue.m_command;
		m_arguments = managerQueue.m_arguments;
		m_flags = managerQueue.m_flags;
		m_delay = managerQueue.m_delay;
		m_delta = managerQueue.m_delta;
		return *this;
	}

	ManagerComponent::ManagerComponent() : Component("Manager", "Manages modules, console commands, and callbacks.") { OnCreate(); }

	ManagerComponent::~ManagerComponent() { OnDestroy(); }

	void ManagerComponent::OnCreate()
	{
		m_queueLocked = false;
		PlaceholderMod = nullptr;
	}

	void ManagerComponent::OnDestroy()
	{
		for (const auto& modulePair : m_modules)
		{
			if (modulePair.second)
			{
				modulePair.second->OnDestroy(); // Give modules a chance to cleanup before all other ones are destroyed and can't be referenced.
			}
		}

		m_modules.clear();
		OnCreate();
	}

	bool ManagerComponent::Initialize()
	{
		if (!IsInitialized())
		{
			// Assigning the "STATES_CasualMatch" and "STATES_RankedMatch" flags, so this module will only be able to be used in casual or ranked games.
			CreateModule<PlaceholderModule>(new PlaceholderModule("Paceholder", "An example module.", States::STATES_CasualMatch | States::STATES_RankedMatch), PlaceholderMod);

			Console.Write(GetNameFormatted() + std::to_string(m_modules.size()) + " Module(s) Initialized!");
			Console.Success(GetNameFormatted() + "Initialized!");
			SetInitialized(true);
		}

		return IsInitialized();
	}

	void ManagerComponent::OnTick()
	{
		if (IsInitialized())
		{
			GRainbowColor::OnTick();

			if (!m_queueLocked)
			{
				if (!m_threadQueue.empty())
				{
					m_queueLocked = true;

					for (const ManagerQueue& queueData : m_threadQueue)
					{
						if (!queueData.IsThreadRaced())
						{
							m_queue.push_back(queueData);
						}
					}

					m_threadQueue.clear();
					m_queueLocked = false;
				}

				if (!m_queue.empty())
				{
					bool safeToClear = true;

					for (ManagerQueue& queueData : m_queue)
					{
						if (!queueData.IsCompleted())
						{
							safeToClear = false;

							if (!queueData.IsThreadRaced()) // If this is ever true, something is going wrong related to a thread racing issue on your end.
							{
								if (!queueData.OnTick())
								{
									continue;
								}

								queueData.SetCompleted(true);

								if (queueData.IsInternal())
								{
									InternalCommand(queueData.GetCommand(), queueData.GetArguments(), ThreadTypes::Main, queueData.ShouldSkipSave());
								}
								else
								{
									ConsoleCommand(queueData.GetCommand(), queueData.GetArguments(), ThreadTypes::Main, queueData.ShouldSkipSave());
								}
							}
						}
					}

					if (safeToClear)
					{
						m_queue.clear();
					}
				}
			}
		}
	}

	void ManagerComponent::OnCanvasDraw(class UCanvas* unrealCanvas)
	{
		if (IsInitialized())
		{
			if (unrealCanvas)
			{
				for (auto& modulePair : m_modules)
				{
					if (modulePair.second)
					{
						modulePair.second->OnCanvasDraw(unrealCanvas);
					}
				}
			}
		}
	}

	std::pair<CommandResults, std::string> ManagerComponent::InternalCommand(const std::string& command, const std::string& arguments, ThreadTypes threadType, bool bSkipSave)
	{
		return ProcessCommandInternal(ManagerQueue(threadType, command, arguments, true, bSkipSave));
	}

	std::pair<CommandResults, std::string> ManagerComponent::InternalCommand(const std::string& command, ThreadTypes threadType)
	{
		return InternalCommand(command, "", threadType);
	}

	void ManagerComponent::ConsoleCommand(const std::string& command, const std::string& arguments, ThreadTypes threadType, bool bSkipSave)
	{
		ProcessCommand(ManagerQueue(threadType, command, arguments, false, bSkipSave));
	}

	void ManagerComponent::ConsoleCommand(const std::string& command, ThreadTypes threadType)
	{
		ConsoleCommand(command, "", threadType);
	}

	void ManagerComponent::AsyncCommand(const std::string& command, const std::string& arguments, uint32_t delay, ThreadTypes threadType)
	{
		ProcessCommand(ManagerQueue(threadType, command, arguments, false, delay));
	}

	void ManagerComponent::AsyncCommand(const std::string& command, uint32_t delay, ThreadTypes threadType)
	{
		ProcessCommand(ManagerQueue(threadType, command, "", false, delay));
	}

	void ManagerComponent::UnrealCommand(std::string unrealCommand, bool bPrintToConsole)
	{
		static AActor* defaultActor = nullptr;

		if (!defaultActor)
		{
			defaultActor = Instances.GetDefaultInstanceOf<AActor>();
		}

		if (defaultActor)
		{
			if (unrealCommand.find("unreal_command") == 0)
			{
				unrealCommand = unrealCommand.erase(0, 15);
			}

			if (bPrintToConsole)
			{
				Console.Write(GetNameFormatted() + "Executing unreal command \"" + unrealCommand + "\".");
			}

			defaultActor->ConsoleCommand(unrealCommand); // May need your own function to convert std::string to FString if your game uses wchar_t.
		}
	}

	template <typename T> std::shared_ptr<T> ManagerComponent::CreateModule(Module* mod, std::shared_ptr<T>& moduleToBind)
	{
		if (mod)
		{
			std::string moduleName = mod->GetName();

			if (!m_modules.contains(moduleName))
			{
				m_modules[moduleName] = std::shared_ptr<Module>(mod);
				moduleToBind = std::static_pointer_cast<T>(m_modules[moduleName]);
				moduleToBind->OnCreateVariables();
				return moduleToBind;
			}
			else
			{
				Console.Warning("[Manager Component] Warning: Duplicate module name detected for \"" + moduleName + "\"!");
			}
		}
		else
		{
			Console.Error("[Manager Component] Error: Failed to create module, given pointer is invalid!");
		}

		return nullptr;
	}

	void ManagerComponent::UpdateAllSettings()
	{
		for (auto& modulePair : m_modules)
		{
			if (modulePair.second)
			{
				modulePair.second->OnSettingChanged(); // Load all your variables with their default values you assigned when creating their setting.
			}
		}
	}

	void ManagerComponent::QueueCommand(ManagerQueue managerQueue)
	{
		if (!managerQueue.IsThreadRaced() && managerQueue.HasCommand())
		{
			if (m_queueLocked)
			{
				std::thread queueThread([this, managerQueue]() {
					while (m_queueLocked)
					{
						std::this_thread::sleep_for(std::chrono::milliseconds(1));
					}

					m_queueLocked = true;
					m_threadQueue.push_back(managerQueue);
					m_queueLocked = false;
				});

				queueThread.detach();
			}
			else
			{
				m_queueLocked = true;
				m_threadQueue.push_back(managerQueue);
				m_queueLocked = false;
			}
		}
	}

	void ManagerComponent::ProcessCommand(const ManagerQueue& managerQueue)
	{
		if (!managerQueue.ShouldProcess())
		{
			QueueCommand(managerQueue);
		}
		else
		{
			std::pair<CommandResults, std::string> returnType = ProcessCommandInternal(managerQueue);

			if (returnType.first != CommandResults::None)
			{
				if (returnType.first == CommandResults::Unrecognized)
				{
					Console.Error(GetNameFormatted() + "Unrecognized command or setting: \"" + managerQueue.GetCommand() + "\".");
				}
				else if (returnType.first == CommandResults::EmptyArguments)
				{
					Console.Error(GetNameFormatted() + "Invalid arguments provided for: \"" + managerQueue.GetCommand() + "\".");
				}
				else if (returnType.first == CommandResults::InvalidArguments)
				{
					Console.Error(GetNameFormatted() + "Arguments are not supported for: \"" + managerQueue.GetCommand() + "\".");
				}
				else if (returnType.first == CommandResults::ModifySetting)
				{
					std::shared_ptr<Setting> consoleSetting = Variables.GetSetting(managerQueue.GetCommand());

					if (consoleSetting)
					{
						Console.Notify(GetNameFormatted() + consoleSetting->GetName() + ": " + returnType.second + " -> " + consoleSetting->GetStringValue());
					}
				}
				else if (returnType.first == CommandResults::PrintSetting)
				{
					std::shared_ptr<Setting> consoleSetting = Variables.GetSetting(managerQueue.GetCommand());

					if (consoleSetting)
					{
						Console.Notify(GetNameFormatted() + "Name: " + consoleSetting->GetName());
						Console.Notify(GetNameFormatted() + "Description: " + consoleSetting->GetDescription());
						Console.Notify(GetNameFormatted() + "Current Value: " + consoleSetting->GetStringValue());
					}
				}
			}
		}
	}

	std::pair<CommandResults, std::string> ManagerComponent::ProcessCommandInternal(const ManagerQueue& managerQueue)
	{
		if (!managerQueue.ShouldProcess())
		{
			QueueCommand(managerQueue);
			return { CommandResults::Queued, "" };
		}
		else
		{
			std::shared_ptr<Command> consoleCommand = Variables.GetCommand(managerQueue.GetCommand());

			if (consoleCommand)
			{
				if (consoleCommand->HasCallback() && !managerQueue.HasArguments())
				{
					consoleCommand->TriggerCallback();
				}
				else if (consoleCommand->HasArgumentCallback())
				{
					if (managerQueue.HasArguments() || !consoleCommand->NeedsArgs())
					{
						consoleCommand->TriggerCallback(managerQueue.GetArguments());
					}
					else
					{
						return { CommandResults::EmptyArguments, "" };
					}
				}
				else
				{
					return { CommandResults::InvalidArguments, "" };
				}
			}
			else
			{
				std::shared_ptr<Setting> consoleSetting = Variables.GetSetting(managerQueue.GetCommand());

				if (consoleSetting && !consoleSetting->IsLocked())
				{
					if (managerQueue.HasArguments())
					{
						std::string oldValue = consoleSetting->GetStringValue();
						consoleSetting->SetStringValue(managerQueue.GetArguments(), ThreadTypes::Main, managerQueue.ShouldSkipSave());
						return { CommandResults::ModifySetting, oldValue };
					}

					return { CommandResults::PrintSetting, "" };
				}

				return { CommandResults::Unrecognized, "" };
			}
		}

		return { CommandResults::None, "" };
	}


	class ManagerComponent Manager;
}