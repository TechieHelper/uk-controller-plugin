#pragma once
#include "Condition.h"

namespace UKControllerPlugin::IntentionCode {

    /**
     * A condition that only passes if the aircraft is arriving at an airfield starting with the specified
     * pattern.
     */
    class ArrivalAirfieldPattern : public Condition
    {
        public:
        ArrivalAirfieldPattern(std::string pattern);
        [[nodiscard]] auto Passes(
            Euroscope::EuroScopeCFlightPlanInterface& flightplan,
            Euroscope::EuroScopeCRadarTargetInterface& radarTarget) -> bool override;

        private:
        // The airfield pattern to match
        std::string pattern;
    };
} // namespace UKControllerPlugin::IntentionCode
