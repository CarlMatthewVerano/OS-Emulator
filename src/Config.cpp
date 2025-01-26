#include "Config.h"
#include "ConfigUtils.h"
#include <fstream>
#include <sstream>
#include <iostream>

Config& Config::getInstance() {
    static Config instance;
    return instance;
}

Config::Config()
    : numCpu(4),
    schedulerType("rr"),
    quantumCycles(5),
    batchProcessFreq(1),
    minIns(1000),
    maxIns(2000),
    delaysPerExec(0),
    maxOverallMem(512),
    memPerFrame(256),
    minMemPerProc(512),
    maxMemPerProc(512) {
}

bool Config::loadConfig(const std::string& filename) {
    std::ifstream configFile(filename);
    if (!configFile) {
        std::cerr << "Failed to open " << filename << std::endl;
        return false;
    }

    std::string line;
    while (std::getline(configFile, line)) {
        // Ignore empty lines and comments
        if (line.empty() || line[0] == '#') continue;

        std::istringstream iss(line);
        std::string paramName;
        iss >> paramName;

        if (paramName == "num-cpu") {
            iss >> numCpu;
            if (numCpu < 1 || numCpu > 128) {
                std::cerr << "Invalid num-cpu in " << filename << ": must be between 1 and 128" << std::endl;
                return false;
            }
        }
        else if (paramName == "scheduler") {
            std::string schedulerValue;
            iss >> schedulerValue;
            schedulerType = ConfigUtils::stripQuotes(schedulerValue);
            if (schedulerType != "fcfs" && schedulerType != "rr") {
                std::cerr << "Invalid scheduler type in " << filename << ": must be 'fcfs' or 'rr'" << std::endl;
                return false;
            }
        }
        else if (paramName == "quantum-cycles") {
            iss >> quantumCycles;
            if (quantumCycles < 1) {
                std::cerr << "Invalid quantum-cycles in " << filename << ": must be greater than 0" << std::endl;
                return false;
            }
        }
        else if (paramName == "batch-process-freq") {
            iss >> batchProcessFreq;
            if (batchProcessFreq < 1) {
                std::cerr << "Invalid batch-process-freq in " << filename << ": must be greater than 0" << std::endl;
                return false;
            }
        }
        else if (paramName == "min-ins") {
            iss >> minIns;
            if (minIns < 1) {
                std::cerr << "Invalid min-ins in " << filename << ": must be greater than 0" << std::endl;
                return false;
            }
        }
        else if (paramName == "max-ins") {
            iss >> maxIns;
            if (maxIns < 1) {
                std::cerr << "Invalid max-ins in " << filename << ": must be greater than 0" << std::endl;
                return false;
            }
        }
        else if (paramName == "delay-per-exec") {
            iss >> delaysPerExec;
            if (delaysPerExec < 0) {
                std::cerr << "Invalid delays-per-exec in " << filename << ": must be non-negative" << std::endl;
                return false;
            }
        }
        else if (paramName == "max-overall-mem") {
            iss >> maxOverallMem;
            if (!ConfigUtils::isInValidRange(maxOverallMem) || !ConfigUtils::isPowerOfTwo(maxOverallMem)) {
                std::cerr << "Invalid max-overall-mem in " << filename << ": must be power of 2 in range [2, 2^32]" << std::endl;
                return false;
            }
        }
        else if (paramName == "mem-per-frame") {
            iss >> memPerFrame;
            if (!ConfigUtils::isInValidRange(memPerFrame) || !ConfigUtils::isPowerOfTwo(memPerFrame)) {
                std::cerr << "Invalid mem-per-frame in " << filename << ": must be power of 2 in range [2, 2^32]" << std::endl;
                return false;
            }
            if (memPerFrame > maxOverallMem) {
                std::cerr << "Invalid mem-per-frame in " << filename << ": must be less than or equal to max-overall-mem" << std::endl;
                return false;
            }
        }
        else if (paramName == "min-mem-per-proc") {
            iss >> minMemPerProc;
            if (!ConfigUtils::isInValidRange(minMemPerProc) || !ConfigUtils::isPowerOfTwo(minMemPerProc)) {
                std::cerr << "Invalid min-mem-per-proc in " << filename << ": must be power of 2 in range [2, 2^32]" << std::endl;
                return false;
            }
        }
        else if (paramName == "max-mem-per-proc") {
            iss >> maxMemPerProc;
            if (!ConfigUtils::isInValidRange(maxMemPerProc) || !ConfigUtils::isPowerOfTwo(maxMemPerProc)) {
                std::cerr << "Invalid max-mem-per-proc in " << filename << ": must be power of 2 in range [2, 2^32]" << std::endl;
                return false;
            }
            if (maxMemPerProc < minMemPerProc) {
                std::cerr << "Invalid max-mem-per-proc in " << filename << ": must be greater than or equal to min-mem-per-proc" << std::endl;
                return false;
            }
        }
        else {
            std::cerr << "Unknown parameter in " << filename << ": " << paramName << std::endl;
            return false;
        }
    }

    configFile.close();
    return true;
}

int Config::getNumCpu() const {
    return numCpu;
}

const std::string& Config::getSchedulerType() const {
    return schedulerType;
}

unsigned int Config::getQuantumCycles() const {
    return quantumCycles;
}

unsigned int Config::getBatchProcessFreq() const {
    return batchProcessFreq;
}

unsigned int Config::getMinIns() const {
    return minIns;
}

unsigned int Config::getMaxIns() const {
    return maxIns;
}

unsigned int Config::getDelaysPerExec() const {
    return delaysPerExec;
}

unsigned int Config::getMaxOverallMem() const {
    return maxOverallMem;
}

unsigned int Config::getMemPerFrame() const {
    return memPerFrame;
}

unsigned int Config::getMinMemPerProc() const {
    return minMemPerProc;
}

unsigned int Config::getMaxMemPerProc() const {
    return maxMemPerProc;
}