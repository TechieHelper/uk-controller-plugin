#include "departure/AircraftDepartedEvent.h"
#include "departure/DepartureMonitor.h"
#include "handoff/HandoffCache.h"
#include "mock/MockEuroScopeCFlightplanInterface.h"
#include "mock/MockEuroScopeCRadarTargetInterface.h"
#include "mock/MockEuroscopePluginLoopbackInterface.h"
#include "test/EventBusTestCase.h"

namespace UKControllerPluginTest::Departure {

    class DepartureMonitorTest : public EventBusTestCase
    {
        public:
        DepartureMonitorTest() : monitor(mockPlugin)
        {
            mockFlightplan = std::make_shared<testing::NiceMock<Euroscope::MockEuroScopeCFlightPlanInterface>>();
            mockRadarTarget = std::make_shared<testing::NiceMock<Euroscope::MockEuroScopeCRadarTargetInterface>>();

            ON_CALL(*mockFlightplan, GetCallsign).WillByDefault(testing::Return("BAW123"));
            ON_CALL(*mockFlightplan, GetOrigin).WillByDefault(testing::Return("EGKK"));

            mockPlugin.AddAllFlightplansItem({mockFlightplan, mockRadarTarget});
        }

        std::shared_ptr<testing::NiceMock<Euroscope::MockEuroScopeCFlightPlanInterface>> mockFlightplan;
        std::shared_ptr<testing::NiceMock<Euroscope::MockEuroScopeCRadarTargetInterface>> mockRadarTarget;
        testing::NiceMock<Euroscope::MockEuroscopePluginLoopbackInterface> mockPlugin;
        UKControllerPlugin::Departure::DepartureMonitor monitor;
    };

    TEST_F(DepartureMonitorTest, ItSendsDepartedEvent)
    {
        ON_CALL(*mockFlightplan, GetDistanceFromOrigin).WillByDefault(testing::Return(2.5));

        ON_CALL(*mockRadarTarget, GetGroundSpeed).WillByDefault(testing::Return(125));

        ON_CALL(*mockRadarTarget, GetFlightLevel).WillByDefault(testing::Return(2500));

        monitor.TimedEventTrigger();

        EXPECT_EQ(1, EventBusObserver().observedEvents.size());
        EXPECT_EQ(
            "BAW123",
            std::any_cast<UKControllerPlugin::Departure::AircraftDepartedEvent>(EventBusObserver().observedEvents[0])
                .callsign);
    }

    TEST_F(DepartureMonitorTest, ItDoesntSendDuplicateDepartedEvents)
    {
        ON_CALL(*mockFlightplan, GetDistanceFromOrigin).WillByDefault(testing::Return(2.5));

        ON_CALL(*mockRadarTarget, GetGroundSpeed).WillByDefault(testing::Return(125));

        ON_CALL(*mockRadarTarget, GetFlightLevel).WillByDefault(testing::Return(2500));

        monitor.TimedEventTrigger();

        EXPECT_EQ(1, EventBusObserver().observedEvents.size());
        EXPECT_EQ(
            "BAW123",
            std::any_cast<UKControllerPlugin::Departure::AircraftDepartedEvent>(EventBusObserver().observedEvents[0])
                .callsign);

        monitor.TimedEventTrigger();
        EXPECT_EQ(1, EventBusObserver().observedEvents.size());
    }

    TEST_F(DepartureMonitorTest, ItAllowsRedepartureIfAircraftGoesOffline)
    {
        ON_CALL(*mockFlightplan, GetDistanceFromOrigin).WillByDefault(testing::Return(2.5));

        ON_CALL(*mockRadarTarget, GetGroundSpeed).WillByDefault(testing::Return(125));

        ON_CALL(*mockRadarTarget, GetFlightLevel).WillByDefault(testing::Return(2500));

        monitor.TimedEventTrigger();
        monitor.FlightPlanDisconnectEvent(*mockFlightplan);
        monitor.TimedEventTrigger();

        EXPECT_EQ(2, EventBusObserver().observedEvents.size());
        EXPECT_EQ(
            "BAW123",
            std::any_cast<UKControllerPlugin::Departure::AircraftDepartedEvent>(EventBusObserver().observedEvents[1])
                .callsign);
    }

    TEST_F(DepartureMonitorTest, ItAllowsRedepartureIfAircraftChangesOriginAirport)
    {
        ON_CALL(*mockFlightplan, GetDistanceFromOrigin).WillByDefault(testing::Return(2.5));

        ON_CALL(*mockRadarTarget, GetGroundSpeed).WillByDefault(testing::Return(125));

        ON_CALL(*mockRadarTarget, GetFlightLevel).WillByDefault(testing::Return(2500));

        monitor.TimedEventTrigger();

        ON_CALL(*mockFlightplan, GetOrigin).WillByDefault(testing::Return("EGLL"));

        monitor.FlightPlanEvent(*mockFlightplan, *mockRadarTarget);
        monitor.TimedEventTrigger();

        EXPECT_EQ(2, EventBusObserver().observedEvents.size());
        EXPECT_EQ(
            "BAW123",
            std::any_cast<UKControllerPlugin::Departure::AircraftDepartedEvent>(EventBusObserver().observedEvents[1])
                .callsign);
    }

    TEST_F(DepartureMonitorTest, ItDoesntAllowRedepartureIfAircraftDoesntChangeOriginAirport)
    {
        ON_CALL(*mockFlightplan, GetDistanceFromOrigin).WillByDefault(testing::Return(2.5));

        ON_CALL(*mockRadarTarget, GetGroundSpeed).WillByDefault(testing::Return(125));

        ON_CALL(*mockRadarTarget, GetFlightLevel).WillByDefault(testing::Return(2500));

        monitor.TimedEventTrigger();
        monitor.FlightPlanEvent(*mockFlightplan, *mockRadarTarget);
        monitor.TimedEventTrigger();

        EXPECT_EQ(1, EventBusObserver().observedEvents.size());
    }

    TEST_F(DepartureMonitorTest, ItDoesntSendDepartedEventNoOrigin)
    {
        ON_CALL(*mockFlightplan, GetOrigin).WillByDefault(testing::Return(""));

        ON_CALL(*mockFlightplan, GetDistanceFromOrigin).WillByDefault(testing::Return(2.5));

        ON_CALL(*mockRadarTarget, GetGroundSpeed).WillByDefault(testing::Return(125));

        ON_CALL(*mockRadarTarget, GetFlightLevel).WillByDefault(testing::Return(2500));

        monitor.TimedEventTrigger();

        EXPECT_EQ(0, EventBusObserver().observedEvents.size());
    }

    TEST_F(DepartureMonitorTest, ItDoesntSendDepartedEventTooFarFromOrigin)
    {
        ON_CALL(*mockFlightplan, GetDistanceFromOrigin).WillByDefault(testing::Return(5.1));

        ON_CALL(*mockRadarTarget, GetGroundSpeed).WillByDefault(testing::Return(125));

        ON_CALL(*mockRadarTarget, GetFlightLevel).WillByDefault(testing::Return(2500));

        monitor.TimedEventTrigger();

        EXPECT_EQ(0, EventBusObserver().observedEvents.size());
    }

    TEST_F(DepartureMonitorTest, ItDoesntSendDepartedEventTooHigh)
    {
        ON_CALL(*mockFlightplan, GetDistanceFromOrigin).WillByDefault(testing::Return(2.5));

        ON_CALL(*mockRadarTarget, GetGroundSpeed).WillByDefault(testing::Return(125));

        ON_CALL(*mockRadarTarget, GetFlightLevel).WillByDefault(testing::Return(5100));

        monitor.TimedEventTrigger();

        EXPECT_EQ(0, EventBusObserver().observedEvents.size());
    }

    TEST_F(DepartureMonitorTest, ItDoesntSendDepartedEventTooLow)
    {
        ON_CALL(*mockFlightplan, GetDistanceFromOrigin).WillByDefault(testing::Return(2.5));

        ON_CALL(*mockRadarTarget, GetGroundSpeed).WillByDefault(testing::Return(125));

        ON_CALL(*mockRadarTarget, GetFlightLevel).WillByDefault(testing::Return(1400));

        monitor.TimedEventTrigger();

        EXPECT_EQ(0, EventBusObserver().observedEvents.size());
    }

    TEST_F(DepartureMonitorTest, ItDoesntSendDepartedEventTooSlow)
    {
        ON_CALL(*mockFlightplan, GetDistanceFromOrigin).WillByDefault(testing::Return(2.5));

        ON_CALL(*mockRadarTarget, GetGroundSpeed).WillByDefault(testing::Return(69));

        ON_CALL(*mockRadarTarget, GetFlightLevel).WillByDefault(testing::Return(2500));

        monitor.TimedEventTrigger();

        EXPECT_EQ(0, EventBusObserver().observedEvents.size());
    }

    TEST_F(DepartureMonitorTest, ItDoesntSendDepartedEventOutOfRange)
    {
        ON_CALL(*mockFlightplan, GetDistanceFromOrigin).WillByDefault(testing::Return(0));

        ON_CALL(*mockRadarTarget, GetGroundSpeed).WillByDefault(testing::Return(125));

        ON_CALL(*mockRadarTarget, GetFlightLevel).WillByDefault(testing::Return(0));

        monitor.TimedEventTrigger();

        EXPECT_EQ(0, EventBusObserver().observedEvents.size());
    }
} // namespace UKControllerPluginTest::Departure
