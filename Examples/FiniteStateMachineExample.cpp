//
// Created by phoenixflower on 11/5/21.
//
#include <iostream>
#include "Primitives/General/mapboxvariant.hpp"
#include "Memory/Optional.h"
#include "Patterns/FiniteStateMachine.hpp"
#include "Core/Unit.hpp"
#include "Definitions/TimeExtension.hpp"
#include "Patterns/DependencyInjection.hpp"
#include <filesystem>
#include <openssl/asn1.h>
#include <openssl/bio.h>
#include <openssl/conf.h>
#include <openssl/err.h>
#include <openssl/pem.h>
#include <openssl/x509.h>
#include "Memory/OptionalReference.h"
#include "Patterns/Configuration.hpp"
namespace LD
{
    namespace Example
    {
        class ApplicationInitializedState;
        class ApplicationHasStartedState;
        class ApplicationFrameHasStartedState;
        class ApplicationExecutionState;
        class ApplicationSleepingState;
        class ApplicationFrameHasEndedState;
        class ApplicationQuittingState;

        using ApplicationState = LD::Variant<
                ApplicationInitializedState,
                ApplicationHasStartedState,
                ApplicationFrameHasStartedState,
                ApplicationExecutionState,
                ApplicationSleepingState,
                ApplicationFrameHasEndedState,
                ApplicationQuittingState>;

        class ApplicationStartedEvent{};
        class ApplicationFrameStartedEvent{};
        class ApplicationExecutionEvent{};
        class ApplicationFrameEndedEvent{};
        class ApplicationSleepingEvent{};
        class ApplicationQuittingEvent{};

        using ApplicationEvent = LD::Variant<
                ApplicationStartedEvent,
                ApplicationFrameStartedEvent,
                ApplicationExecutionEvent,
                ApplicationSleepingEvent,
                ApplicationFrameEndedEvent,
                ApplicationQuittingEvent>;


        class ApplicationInitializedState
        {
        public:
            template<typename Event>
            LD::Optional<ApplicationState> operator()(Event && event)  noexcept;
        };


        class ApplicationHasStartedState
        {
        public:
            template<typename Event>
            LD::Optional<ApplicationState> operator()(Event && event)  noexcept;
        };

        class ApplicationFrameHasStartedState
        {
        public:
            template<typename Event>
            LD::Optional<ApplicationState> operator()(Event && event)  noexcept;

        };

        class ApplicationExecutionState
        {
        public:
            template<typename Event>
            LD::Optional<ApplicationState> operator()(Event && event)  noexcept;
        };

        class ApplicationSleepingState
        {
        public:
            template<typename Event>
            LD::Optional<ApplicationState> operator()(Event && event)  noexcept;
        };

        class ApplicationFrameHasEndedState
        {
        public:
            template<typename Event>
            LD::Optional<ApplicationState> operator()(Event  event)  noexcept;

            LD::Optional<ApplicationState> operator()( ApplicationQuittingEvent)  noexcept;
        };

        class ApplicationQuittingState
        {
        public:
            template<typename Event>
            LD::Optional<ApplicationState> operator()(Event && event)  noexcept;
        };


        template<typename Event>
        LD::Optional<ApplicationState> ApplicationInitializedState::operator()(Event &&event)  noexcept
        {
            //printf("Application Has Initialized State\n");
            return ApplicationState{ApplicationHasStartedState{}};
        }
        template<typename Event>
        LD::Optional<ApplicationState> ApplicationHasStartedState::operator()(Event &&event)  noexcept
        {
            //printf("Application Has Started State\n");
            return ApplicationState{ApplicationFrameHasStartedState{}};
        }


        template<typename Event>
        LD::Optional<ApplicationState> ApplicationFrameHasStartedState::operator()(Event &&event)  noexcept
        {
            if constexpr(LD::CT::IsSame(LD::Type<Event>{},LD::Type<ApplicationQuittingEvent>{}))
            {
                return ApplicationState{ApplicationQuittingState{}};
            }
            //("Application Frame Has Started State\n");
            return ApplicationState{ApplicationExecutionState{}};
        }


        template<typename Event>
        LD::Optional<ApplicationState> ApplicationExecutionState::operator()(Event &&event)  noexcept
        {
            //printf("Application Execution State\n");
            return ApplicationState{ApplicationSleepingState{}};;
        }

        template<typename Event>
        LD::Optional<ApplicationState> ApplicationSleepingState::operator()(Event &&event)  noexcept
        {
            //printf("Application Sleeping State\n");
            return ApplicationState{ApplicationFrameHasEndedState{}};
        }

        template<typename Event>
        LD::Optional<ApplicationState> ApplicationFrameHasEndedState::operator()(Event  event)  noexcept
        {
            return ApplicationState{ApplicationFrameHasStartedState{}};
        }

        LD::Optional<ApplicationState> ApplicationFrameHasEndedState::operator()(ApplicationQuittingEvent)  noexcept
        {
            //printf("Application is Quitting\n");
            return ApplicationState{ApplicationQuittingState{}};
        }

        template<typename Event>
        LD::Optional<ApplicationState> ApplicationQuittingState::operator()(Event &&event)  noexcept
        {
            return LD::nullopt;
        }



        class StateTransitionExecutor
        {
        private:
        public:

            template<typename U, typename V>
            void operator()(U &&, V && ) noexcept
            {
            }
            void operator()(ApplicationInitializedState,ApplicationHasStartedState)
            {
                //printf("Transitioned from Initialized to Application Has Started\n");
            }

            void operator()(ApplicationHasStartedState,ApplicationFrameHasStartedState)
            {
                //printf("Transitioned from Application Started to Application Frame Has Started\n");
            }

            void operator()(ApplicationFrameHasStartedState,ApplicationExecutionState)
            {
                //printf("Transitioned from Application Frame Started to Application Execution State\n");
            }

            void operator()(ApplicationExecutionState,ApplicationSleepingState)
            {
                //printf("Transitioned from Application Execution State to Application Sleeping State\n");
            }

            void operator()(ApplicationSleepingState,ApplicationFrameHasEndedState)
            {
                //printf("Transitioned from Application Sleeping State to Application FrameHasEnded State\n");
            }
            void operator()(ApplicationFrameHasEndedState,ApplicationFrameHasStartedState)
            {
                //printf("Transitioned from Application FrameHasEnded State to Application FrameHasStarted State\n");
            }

            void operator()(ApplicationFrameHasEndedState,ApplicationQuittingState)
            {
                //printf("Transitioned from Application FrameHasEnded State to Application Quitting State\n");
            }

            void operator()(ApplicationHasStartedState,ApplicationQuittingState)
            {

            }
            void operator()(ApplicationInitializedState,ApplicationQuittingState)
            {

            }

        };

        class StateExecutorBase
        {
        private:

        public:
            virtual void operator()(ApplicationInitializedState) noexcept = 0;
            virtual void operator()(ApplicationHasStartedState) noexcept= 0;
            virtual void operator()(ApplicationFrameHasStartedState) noexcept= 0;
            virtual void operator()(ApplicationExecutionState)noexcept = 0;
            virtual void operator()(ApplicationFrameHasEndedState) noexcept= 0;
            virtual LD::Second<LD::Float> operator()(ApplicationSleepingState) noexcept= 0;
            virtual void operator()(ApplicationQuittingState) noexcept= 0;
        };
        class StateExecutor
        {
        private:
        public:
            void operator()(ApplicationInitializedState)
            {

            }
            void operator()(ApplicationHasStartedState)
            {

            }

            void operator()(ApplicationFrameHasStartedState)
            {

            }

            void operator()(ApplicationExecutionState)
            {

            }

            void operator()(ApplicationFrameHasEndedState)
            {

            }

            void operator()(ApplicationSleepingState)
            {
                LD::SleepFor(1_s);
            }


            void operator()(ApplicationQuittingState)
            {

            }
        };


        template<typename Executor>
        class BasicStateExecutor
        {
        private:
            Executor mExecutor;
        public:
            BasicStateExecutor(Executor && executor) noexcept:mExecutor{LD::Forward<Executor>(executor)}
            {

            }
            void operator()(ApplicationInitializedState state)
            {
                this->mExecutor(state);
            }
            void operator()(ApplicationHasStartedState state)
            {
                this->mExecutor(state);
            }

            void operator()(ApplicationFrameHasStartedState state)
            {
                this->mExecutor(state);
            }

            void operator()(ApplicationExecutionState state)
            {
                this->mExecutor(state);
            }

            void operator()(ApplicationFrameHasEndedState state)
            {
                this->mExecutor(state);
            }

            void operator()(ApplicationSleepingState state)
            {
                LD::SleepFor(this->mExecutor(state));
            }


            void operator()(ApplicationQuittingState state)
            {
                this->mExecutor(state);
            }
        };

        class PolymorhpicStateExecutor: public LD::DependencyInjection<PolymorhpicStateExecutor(StateExecutorBase&)>
        {
        private:
            StateExecutorBase & mExecutor;
        public:
            PolymorhpicStateExecutor(StateExecutorBase & polymorphicExecutor) noexcept:mExecutor{polymorphicExecutor}
            {

            }
            void operator()(ApplicationInitializedState state)
            {
                this->mExecutor(state);
            }
            void operator()(ApplicationHasStartedState state)
            {
                this->mExecutor(state);
            }

            void operator()(ApplicationFrameHasStartedState state)
            {
                this->mExecutor(state);
            }

            void operator()(ApplicationExecutionState state)
            {
                this->mExecutor(state);
            }

            void operator()(ApplicationFrameHasEndedState state)
            {
                this->mExecutor(state);
            }

            void operator()(ApplicationSleepingState state)
            {
                LD::SleepFor(this->mExecutor(state));
            }


            void operator()(ApplicationQuittingState state)
            {
                this->mExecutor(state);
            }
        };


        class EventGeneratorBase
        {
        private:
        public:
            virtual auto operator()(ApplicationInitializedState) noexcept -> LD::Optional<ApplicationEvent> = 0;

            virtual auto  operator()(ApplicationHasStartedState) noexcept -> LD::Optional<ApplicationEvent> = 0;

            virtual auto  operator()(ApplicationFrameHasStartedState) noexcept -> LD::Optional<ApplicationEvent> = 0;

            virtual auto  operator()(ApplicationExecutionState) noexcept -> LD::Optional<ApplicationEvent> = 0;

            virtual auto  operator()(ApplicationSleepingState) noexcept -> LD::Optional<ApplicationEvent> = 0;

            virtual auto  operator()(ApplicationFrameHasEndedState) noexcept -> LD::Optional<ApplicationEvent> = 0;

            virtual auto  operator()(ApplicationQuittingState) noexcept -> LD::Optional<ApplicationEvent> = 0;
        };
        class EventExecutor
        {
        private:
            LD::UInteger mCycleCount;
        public:
            EventExecutor() noexcept:mCycleCount{0}
            {

            }
            auto operator()(ApplicationInitializedState) noexcept -> LD::Optional<ApplicationEvent>
            {
                //todo add entry point to determine if the application should quit at this point
                return ApplicationEvent{ApplicationStartedEvent{}};
            }
            auto  operator()(ApplicationHasStartedState) noexcept -> LD::Optional<ApplicationEvent>
            {
                //todo add entry point to determine if the application should quit at this point
                return ApplicationEvent{ApplicationFrameStartedEvent{}};
            }

            auto  operator()(ApplicationFrameHasStartedState) noexcept -> LD::Optional<ApplicationEvent>
            {
                //todo add entry point to determine if the application should quit at this point
                return ApplicationEvent{ApplicationExecutionEvent{}};
            }

            auto  operator()(ApplicationExecutionState) noexcept -> LD::Optional<ApplicationEvent>
            {
                //todo add entry point to determine if the application should quit at this point
                return ApplicationEvent{ApplicationSleepingEvent{}};
            }

            auto  operator()(ApplicationSleepingState) noexcept -> LD::Optional<ApplicationEvent>
            {
                return ApplicationEvent{ApplicationFrameEndedEvent{}};
            }

            auto  operator()(ApplicationFrameHasEndedState) noexcept -> LD::Optional<ApplicationEvent>
            {
                if (this->mCycleCount == 5)
                {
                    return ApplicationEvent{ApplicationQuittingEvent{}};
                }
                this->mCycleCount++;
                return ApplicationEvent{ApplicationFrameStartedEvent{}};
            }
            auto  operator()(ApplicationQuittingState) noexcept -> LD::Optional<ApplicationEvent>
            {
                return LD::nullopt;
            }
        };

        class PolymorhpicEventExecutor: public LD::DependencyInjection<PolymorhpicStateExecutor(EventGeneratorBase&)>
        {
        private:
            EventGeneratorBase & mExecutor;
        public:
            PolymorhpicEventExecutor(EventGeneratorBase & executor):mExecutor{executor}
            {

            }

            auto operator()(ApplicationInitializedState state) noexcept -> LD::Optional<ApplicationEvent>
            {
                return this->mExecutor(state);
            }
            auto  operator()(ApplicationHasStartedState state) noexcept -> LD::Optional<ApplicationEvent>
            {
                return this->mExecutor(state);
            }

            auto  operator()(ApplicationFrameHasStartedState state) noexcept -> LD::Optional<ApplicationEvent>
            {
                return this->mExecutor(state);
            }

            auto  operator()(ApplicationExecutionState state) noexcept -> LD::Optional<ApplicationEvent>
            {
                return this->mExecutor(state);
            }

            auto  operator()(ApplicationSleepingState state) noexcept -> LD::Optional<ApplicationEvent>
            {
                return this->mExecutor(state);
            }

            auto  operator()(ApplicationFrameHasEndedState state) noexcept -> LD::Optional<ApplicationEvent>
            {
                return this->mExecutor(state);
            }
            auto  operator()(ApplicationQuittingState state) noexcept -> LD::Optional<ApplicationEvent>
            {
                return this->mExecutor(state);
            }
        };

        template<typename Executor>
        class BasicEventExecutor
        {
        private:
            Executor mExecutor;
        public:
            BasicEventExecutor(Executor && executor) noexcept:mExecutor{LD::Forward<Executor>(executor)}
            {

            }
            auto operator()(ApplicationInitializedState state) noexcept -> LD::Optional<ApplicationEvent>
            {
                return this->mExecutor(state);
            }
            auto  operator()(ApplicationHasStartedState state) noexcept -> LD::Optional<ApplicationEvent>
            {
                return this->mExecutor(state);
            }

            auto  operator()(ApplicationFrameHasStartedState state) noexcept -> LD::Optional<ApplicationEvent>
            {
                return this->mExecutor(state);
            }

            auto  operator()(ApplicationExecutionState state) noexcept -> LD::Optional<ApplicationEvent>
            {
                return this->mExecutor(state);
            }

            auto  operator()(ApplicationSleepingState state) noexcept -> LD::Optional<ApplicationEvent>
            {
                return this->mExecutor(state);
            }

            auto  operator()(ApplicationFrameHasEndedState state) noexcept -> LD::Optional<ApplicationEvent>
            {
                return this->mExecutor(state);
            }
            auto  operator()(ApplicationQuittingState state) noexcept -> LD::Optional<ApplicationEvent>
            {
                return this->mExecutor(state);
            }
        };


        enum class FileStatus {created, modified, erased};
        template<typename Executor>
        class FileWatcher: public StateExecutorBase
        {
        private:
            Executor mExecutor;
            std::unordered_map<std::string, std::filesystem::file_time_type> paths_;
            std::string path_to_watch;
        public:


            FileWatcher(std::string path_to_watch, Executor && exeuctor) : path_to_watch{path_to_watch},mExecutor{exeuctor}
            {
                for(auto &file : std::filesystem::recursive_directory_iterator(path_to_watch))
                {
                    paths_[file.path().string()] = std::filesystem::last_write_time(file);

                }
            }
            virtual void operator()(ApplicationInitializedState) noexcept
            {

            }
            virtual void operator()(ApplicationHasStartedState) noexcept
            {

            }
            virtual void operator()(ApplicationFrameHasStartedState) noexcept
            {

            }
            virtual void operator()(ApplicationExecutionState)noexcept
            {
                printf("being called\n");
                auto it = paths_.begin();
                while (it != paths_.end())
                {
                    if (!std::filesystem::exists(it->first))
                    {
                        mExecutor(LD::StringView{it->first}, FileStatus::erased);
                        it = paths_.erase(it);
                    }
                    else
                    {
                        it++;
                    }

                    for(auto &file : std::filesystem::recursive_directory_iterator(path_to_watch))
                    {
                        auto current_file_last_write_time = std::filesystem::last_write_time(file);
                        if(!contains(file.path().string()))
                        {
                            paths_[file.path().string()] = current_file_last_write_time;
                            mExecutor(LD::StringView{file.path().string()}, FileStatus::created);
                        }else
                        {
                            if(paths_[file.path().string()] != current_file_last_write_time)
                            {
                                paths_[file.path().string()] = current_file_last_write_time;
                                mExecutor(LD::StringView{file.path().string()}, FileStatus::modified);
                            }
                        }
                    }

                }
            }
            virtual void operator()(ApplicationFrameHasEndedState) noexcept
            {

            }
            virtual LD::Second<LD::Float> operator()(ApplicationSleepingState) noexcept
            {
                return 1_s;
            }
            virtual void operator()(ApplicationQuittingState) noexcept
            {

            }

            bool contains(const std::string &key)
            {
                auto el = paths_.find(key);

                return el != paths_.end();
            }

         };

        extern void FiniteStateMachineExample()
        {
            auto j2 = R"(
                {
                "happy": true,
                "pi": 3.141,
                "key": 12.97,
                "object" : {"key": 72 }
                }
            )"_json;
            nlohmann::json mConfiguration{j2};

            LD::JsonConfiguration configuration(LD::Mem::GetNewDeleteResourceReference(),mConfiguration);

            //LD::Configuration & config = (Configuration &) configuration;
            auto subConfiguration =  configuration("object",LD::Type<LD::Configuration>{});
            LD::Optional<LD::Integer> possibleRes = (*(*subConfiguration))("key",LD::Type<LD::Integer>{});

            std::cout << "SubjectObject Key: " << *possibleRes << "\n";
            LD::BasicConfiguration basicConfig{configuration};

            LD::Optional<LD::Float> optionalFloatingPointNumber;
            basicConfig["key"]  >> optionalFloatingPointNumber;

            if (optionalFloatingPointNumber)
            {
                std::cout << "Possible Value 1: " << *optionalFloatingPointNumber << "\n";
            }
            LD::Optional<LD::Float> possibleValue = configuration("key",LD::Type<LD::Float>{});
            if (possibleValue)
            {
                std::cout << "Possible Value: " << *possibleValue << "\n";
            }
            auto it = mConfiguration.find("");

            LD::Optional<LD::ElementReference<int>> optional;


            auto fileWatcherFunctor = [](auto filePath,auto) noexcept {std::cout << filePath << "\n";};
            FileWatcher<decltype(fileWatcherFunctor)&> watcher("/home/phoenixflower/Desktop/",fileWatcherFunctor);

            PolymorhpicStateExecutor polymorhpicStateExecutor{watcher};
            StateTransitionExecutor stateTransitionExecutor;

            StateExecutor stateExecutor;
            auto stateManagerExecutor = LD::Overload{stateTransitionExecutor,polymorhpicStateExecutor};
            LD::StateManager<decltype(stateManagerExecutor)&,ApplicationState(ApplicationStartedEvent)> stateManager{stateManagerExecutor};

            EventExecutor eventGenerator;

            auto executors = LD::Overload{eventGenerator,stateTransitionExecutor,polymorhpicStateExecutor};


            using FSM = LD::StateManager<decltype(stateManagerExecutor)&,ApplicationState(ApplicationStartedEvent)>;
            using Evnt =  EventExecutor;
            //ApplicationState
            LD::EventDispatcher<FSM&(), Evnt&(ApplicationState)> eventDispatcher
            {
                stateManager,
                eventGenerator
            };

            auto runningPredicate = [](auto && state) noexcept
            {
                return true;
            };
            auto quittingPredicate = [](ApplicationQuittingState) noexcept
            {
                return false;
            };
            //we want to quit when the quitting state has been achieved
            while (LD::Visit(LD::Overload{runningPredicate,quittingPredicate},stateManager.State()))
            {
                eventDispatcher();
            }
        }
    }
}
