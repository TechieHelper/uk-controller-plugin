#include "PersistenceContainer.h"
#include "aircraft/AircraftTypeCollection.h"
#include "aircraft/AircraftTypeMapperInterface.h"
#include "aircraft/CallsignSelectionListFactory.h"
#include "airfield/AirfieldCollection.h"
#include "api/ApiFactory.h"
#include "api/ApiInterface.h"
#include "bootstrap/BootstrapProviderCollection.h"
#include "bootstrap/ModuleFactories.h"
#include "command/CommandHandlerCollection.h"
#include "controller/ActiveCallsignCollection.h"
#include "controller/ControllerPositionCollection.h"
#include "controller/ControllerPositionHierarchyFactory.h"
#include "controller/ControllerStatusEventHandlerCollection.h"
#include "controller/HandoffEventHandlerCollection.h"
#include "countdown/CountdownTimer.h"
#include "countdown/TimerConfigurationManager.h"
#include "curl/CurlInterface.h"
#include "datablock/DisplayTime.h"
#include "dependency/DependencyLoaderInterface.h"
#include "dialog/DialogManager.h"
#include "euroscope/RadarTargetEventHandlerCollection.h"
#include "euroscope/RunwayDialogAwareCollection.h"
#include "euroscope/UserSetting.h"
#include "euroscope/UserSettingAwareCollection.h"
#include "flightplan/FlightPlanEventHandlerCollection.h"
#include "flightplan/StoredFlightplanCollection.h"
#include "flightrule/FlightRuleCollection.h"
#include "graphics/GdiGraphicsWrapper.h"
#include "graphics/GdiplusBrushes.h"
#include "historytrail/HistoryTrailRepository.h"
#include "hold/AbstractHoldLevelRestriction.h"
#include "hold/DeemedSeparatedHold.h"
#include "hold/HoldDisplayFactory.h"
#include "hold/HoldManager.h"
#include "hold/HoldSelectionMenu.h"
#include "hold/PublishedHoldCollection.h"
#include "initialaltitude/InitialAltitudeEventHandler.h"
#include "integration/ExternalMessageEventHandler.h"
#include "intention/IntentionCodeCache.h"
#include "intention/IntentionCodeGenerator.h"
#include "intention/SectorExitRepository.h"
#include "list/PopupListFactory.h"
#include "login/Login.h"
#include "message/UserMessager.h"
#include "metar/MetarEventHandlerCollection.h"
#include "minstack/MinStackManager.h"
#include "navaids/NavaidCollection.h"
#include "notifications/NotificationsMenuItem.h"
#include "ownership/AirfieldServiceProviderCollection.h"
#include "plugin/FunctionCallEventHandler.h"
#include "plugin/UKPlugin.h"
#include "push/PushEventProcessorCollection.h"
#include "radarscreen/RadarRenderableCollection.h"
#include "radarscreen/RadarScreenFactory.h"
#include "radarscreen/ScreenControls.h"
#include "regional/RegionalPressureManager.h"
#include "releases/DepartureReleaseEventHandler.h"
#include "runway/RunwayCollection.h"
#include "sectorfile/RunwayCollection.h"
#include "setting/SettingRepository.h"
#include "sid/SidCollection.h"
#include "sid/SidMapperInterface.h"
#include "squawk/SquawkAssignment.h"
#include "squawk/SquawkEventHandler.h"
#include "squawk/SquawkGenerator.h"
#include "tag/TagItemCollection.h"
#include "task/TaskRunnerInterface.h"
#include "timedevent/TimedEventCollection.h"
#include "wake/WakeSchemeCollection.h"
#include "windows/WinApiInterface.h"

namespace UKControllerPlugin::Bootstrap {
    PersistenceContainer::PersistenceContainer() = default;
    PersistenceContainer::~PersistenceContainer() = default;
    PersistenceContainer::PersistenceContainer(PersistenceContainer&&) noexcept = default;
    auto PersistenceContainer::operator=(PersistenceContainer&&) noexcept -> PersistenceContainer& = default;
} // namespace UKControllerPlugin::Bootstrap
