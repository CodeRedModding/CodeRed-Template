#include "Manager.hpp"
#include "../Includes.hpp"

namespace CodeRed
{
	ManagerQueue::ManagerQueue(const std::string& command, const std::string& arguments, bool bInternal, bool bSkipSave) :
		Thread(ThreadTypes::Main),
		Command(command),
		Arguments(arguments),
		Delay(0),
		Delta(0),
		SkipSave(bSkipSave),
		Internal(bInternal),
		Completed(false)
	{

	}

	ManagerQueue::ManagerQueue(const std::string& command, const std::string& arguments, bool bInternal, uint64_t delay, uint32_t multiplier, bool bSkipSave) :
		Thread(ThreadTypes::Main),
		Command(command),
		Arguments(arguments),
		Delay(delay* multiplier),
		Delta(0),
		SkipSave(bSkipSave),
		Internal(bInternal),
		Completed(false)
	{

	}

	ManagerQueue::ManagerQueue(const ManagerQueue& managerQueue) :
		Thread(managerQueue.Thread),
		Command(managerQueue.Command),
		Arguments(managerQueue.Arguments),
		Delay(managerQueue.Delay),
		Delta(managerQueue.Delta),
		SkipSave(managerQueue.SkipSave),
		Internal(managerQueue.Internal),
		Completed(managerQueue.Completed)
	{

	}

	ManagerQueue::~ManagerQueue() {}

	bool ManagerQueue::HasArguments() const
	{
		return (!Arguments.empty());
	}

	bool ManagerQueue::IsAsync() const
	{
		return (Delay > 0);
	}

	bool ManagerQueue::OnTick()
	{
		if (IsAsync())
		{
			Delta++;
			return (Delta >= Delay);
		}

		return true;
	}

	ManagerQueue& ManagerQueue::operator=(const ManagerQueue& managerQueue)
	{
		Thread = managerQueue.Thread;
		Command = managerQueue.Command;
		Arguments = managerQueue.Arguments;
		Delay = managerQueue.Delay;
		Delta = managerQueue.Delta;
		SkipSave = managerQueue.SkipSave;
		Internal = managerQueue.Internal;
		Completed = managerQueue.Completed;
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
		GRainbowColor::OnTick();

		if (!m_queueLocked)
		{
			if (!m_threadQueue.empty())
			{
				for (ManagerQueue& data : m_threadQueue)
				{
					m_queue.push_back(data);
				}

				m_threadQueue.clear();
			}

			if (!m_queue.empty())
			{
				bool safeToClear = true;

				for (ManagerQueue& data : m_queue)
				{
					if (!data.Completed)
					{
						safeToClear = false;

						if (data.IsAsync())
						{
							if (data.OnTick())
							{
								data.Delay = 0;
							}
							else
							{
								continue;
							}
						}

						data.Completed = true;

						if ((data.Thread == ThreadTypes::Main) || (data.Thread == ThreadTypes::Render))
						{
							if (data.Internal)
							{
								InternalCommand(data.Command, data.Arguments, ThreadTypes::Main, data.SkipSave);
							}
							else
							{
								ConsoleCommand(data.Command, data.Arguments, ThreadTypes::Main, data.SkipSave);
							}
						}
						else
						{
							return; // If this ever happens, something is going wrong related to a thread racing issue on your end.
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

	void ManagerComponent::OnCanvasDraw(class UCanvas* unrealCanvas)
	{
		OnTick();

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

	std::pair<CommandResults, std::string> ManagerComponent::InternalCommand(const std::string& command, const std::string& arguments, ThreadTypes thread, bool bSkipSave)
	{
		return ProcessCommandInternal(ManagerQueue(command, arguments, true, bSkipSave), thread);
	}

	std::pair<CommandResults, std::string> ManagerComponent::InternalCommand(const std::string& command, ThreadTypes thread)
	{
		return InternalCommand(command, "", thread);
	}

	void ManagerComponent::ConsoleCommand(const std::string& command, const std::string& arguments, ThreadTypes thread, bool bSkipSave)
	{
		ProcessCommand(ManagerQueue(command, arguments, false, bSkipSave), thread);
	}

	void ManagerComponent::ConsoleCommand(const std::string& command, ThreadTypes thread)
	{
		ConsoleCommand(command, "", thread);
	}

	void ManagerComponent::AsyncCommand(const std::string& command, const std::string& arguments, uint64_t delay, ThreadTypes thread)
	{
		ProcessCommand(ManagerQueue(command, arguments, false, delay), thread);
	}

	void ManagerComponent::AsyncCommand(const std::string& command, uint64_t delay, ThreadTypes thread)
	{
		ProcessCommand(ManagerQueue(command, "", false, delay), thread);
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

	void ManagerComponent::QueueCommand(const ManagerQueue& managerQueue, ThreadTypes thread)
	{
		if (!managerQueue.Command.empty())
		{
			ManagerQueue queueCopy = managerQueue;
			queueCopy.Thread = thread;

			if (m_queueLocked)
			{
				std::thread queueThread([this, queueCopy]() {
					while (m_queueLocked)
					{
						std::this_thread::sleep_for(std::chrono::milliseconds(1));
					}

					m_queueLocked = true;
					m_threadQueue.push_back(queueCopy);
					m_queueLocked = false;
				});

				queueThread.detach();
			}
			else
			{
				m_queueLocked = true;
				m_threadQueue.push_back(queueCopy);
				m_queueLocked = false;
			}
		}
	}

	void ManagerComponent::ProcessCommand(const ManagerQueue& managerQueue, ThreadTypes thread)
	{
		if (thread != ThreadTypes::Main)
		{
			if (!managerQueue.Command.empty())
			{
				QueueCommand(managerQueue, thread);
			}
		}
		else
		{
			std::pair<CommandResults, std::string> returnType = ProcessCommandInternal(managerQueue, thread);

			if (returnType.first != CommandResults::None)
			{
				if (returnType.first == CommandResults::Unrecognized)
				{
					Console.Error(GetNameFormatted() + "Unrecognized command or setting: \"" + managerQueue.Command + "\".");
				}
				else if (returnType.first == CommandResults::EmptyArguments)
				{
					Console.Error(GetNameFormatted() + "Invalid arguments provided for: \"" + managerQueue.Command + "\".");
				}
				else if (returnType.first == CommandResults::InvalidArguments)
				{
					Console.Error(GetNameFormatted() + "Arguments are not supported for: \"" + managerQueue.Command + "\".");
				}
				else if (returnType.first == CommandResults::ModifySetting)
				{
					std::shared_ptr<Setting> consoleSetting = Variables.GetSetting(managerQueue.Command);

					if (consoleSetting)
					{
						Console.Notify(GetNameFormatted() + consoleSetting->GetName() + ": " + returnType.second + " -> " + consoleSetting->GetStringValue());
					}
				}
				else if (returnType.first == CommandResults::PrintSetting)
				{
					std::shared_ptr<Setting> consoleSetting = Variables.GetSetting(managerQueue.Command);

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

	std::pair<CommandResults, std::string> ManagerComponent::ProcessCommandInternal(const ManagerQueue& managerQueue, ThreadTypes thread)
	{
		if ((thread != ThreadTypes::Main) || (managerQueue.IsAsync()))
		{
			if (!managerQueue.Command.empty())
			{
				QueueCommand(managerQueue, thread);
				return { CommandResults::Queued, "" };
			}
		}
		else
		{
			std::shared_ptr<Command> consoleCommand = Variables.GetCommand(managerQueue.Command);

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
						consoleCommand->TriggerCallback(managerQueue.Arguments);
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
				std::shared_ptr<Setting> consoleSetting = Variables.GetSetting(managerQueue.Command);

				if (consoleSetting && !consoleSetting->IsLocked())
				{
					if (managerQueue.HasArguments())
					{
						std::string oldValue = consoleSetting->GetStringValue();
						consoleSetting->SetStringValue(managerQueue.Arguments, ThreadTypes::Main, managerQueue.SkipSave);
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