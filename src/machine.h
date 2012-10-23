#ifndef MACHINE_H_
#define MACHINE_H_

#include <unordered_set>
#include <string>
#include <map>
#include <mutex>

#include "utils.h"
#include "interfaces.h"
#include "software.h"

class Job;

typedef std::unordered_set<Software, Software::Hash> SoftwareSet;

class Machine : public ISave, public IUpdate
{
public:
    Machine(const std::string& machineName, uint maxJobs, double totalRAM, double totalDiskSpace)
        : _name(machineName), _maxJobs(maxJobs), _totalRAM(totalRAM), _totalDiskSpace(totalDiskSpace),
          _availableDiskSpace(totalDiskSpace), _availableRAM(totalRAM) {}

    virtual ~Machine();

    const std::string& GetMachineName() const { return _name; }
    uint GetMaxJobs() const { return _maxJobs; }
    uint GetCurrentJobs() const;
    double GetAvailableRAM() const { return _availableRAM; }
    double GetAvailableDiskSpace() const { return _availableDiskSpace; }
    double GetTotalRAM() const { return _totalRAM; }
    double GetTotalDiskSpace() const { return _totalDiskSpace; }

    void AddRequiredSoftware(const Software& sw) { _availableSoftware.insert(sw); }
    bool IsRequiredSoftware(const Software& sw) const { return _availableSoftware.find(sw) != _availableSoftware.end(); }

    bool AddJob(Job* job);
    bool RemoveJob(uint id);

    bool Save(ByteBuffer& bb) const override;
    static Machine* Load(ByteBuffer& bb);

    void Update(uint diff);

private:
    bool SoftwareMeetsRequirements(const Software& sw) const;

    double _availableRAM;
    double _availableDiskSpace;
    const double _totalRAM;
    const double _totalDiskSpace;
    const uint _maxJobs;

    static uint _lastJobId;

    SoftwareSet _availableSoftware;
    std::map<uint, Job*> _currentJobs;

    const std::string _name;

    mutable std::mutex _mutex;

private: // no copying
    Machine(const Machine&);
    Machine& operator =(Machine const&);
};

#endif // MACHINE_H_
