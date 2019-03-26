#pragma once

namespace Sentinel {
namespace Stat {

class Node {
public:
    virtual ~Node() {}

    /**
     * Get incoming request per minute ({@code pass + block}).
     *
     * @return total request count per minute
     */
    virtual long TotalCountInMinute() = 0;

    /**
     * Get pass count per minute.
     *
     * @return total passed request count per minute
     */
    virtual long PassCountInMinute() = 0;

    /**
     * Get {@link Entry#exit()} count per minute.
     *
     * @return total completed request count per minute
     */
    virtual long CompleteCountInMinute() = 0;

    /**
     * Get blocked request count per minute (totalBlockRequest).
     *
     * @return total blocked request count per minute
     */
    virtual long BlockCountInMinute() = 0;

    /**
     * Get exception count per minute.
     *
     * @return total business exception count per minute
     */
    virtual long ExceptionCountInMinute() = 0;

    /**
     * Get pass request per second.
     *
     * @return QPS of passed requests
     */
    virtual double PassQps() = 0;

    /**
     * Get block request per second.
     *
     * @return QPS of blocked requests
     */
    virtual double BlockQps() = 0;

    /**
     * Get {@link #passQps()} + {@link #blockQps()} request per second.
     *
     * @return QPS of passed and blocked requests
     */
    virtual double TotalQps() = 0;

    /**
     * Get {@link Entry#exit()} request per second.
     *
     * @return QPS of completed requests
     */
    virtual double CompleteQps() = 0;

    /**
     * Get estimated max success QPS till now.
     *
     * @return max completed QPS
     */
    virtual double MaxCompleteQps() = 0;

    /**
     * Get exception count per second.
     *
     * @return QPS of exception occurs
     */
    virtual double ExceptionQps() = 0;

    /**
     * Get average rt per second.
     *
     * @return average response time per second
     */
    virtual double AvgRt() = 0;

    /**
     * Get minimal response time.
     *
     * @return recorded minimal response time
     */
    virtual double MinRt() = 0;

    /**
     * Get current active thread count.
     *
     * @return current active thread count
     */
    virtual int CurThreadNum() = 0;

    /**
     * Get last second block QPS.
     */
    virtual double PreviousBlockQps() = 0;

    /**
     * Last window QPS.
     */
    virtual double PreviousPassQps() = 0;

    /**
     * Fetch all valid metric nodes of resources.
     *
     * @return valid metric nodes of resources
     */
    //virtual std::map&<long, MetricNode> metrics();

    /**
     * Add pass count.
     *
     * @param count count to add pass
     */
    virtual void AddPassRequest(int count) = 0;

    /**
     * Add RT and complete count.
     *
     * @param rt      response time
     */
    virtual void AddRtAndCompleteRequest(long rt, int completeCount) = 0;

    /**
     * Increase the block count.
     *
     * @param count count to add
     */
    virtual void AddBlockRequest(int count) = 0;

    /**
     * Add the biz exception count.
     *
     * @param count count to add
     */
    virtual void AddExceptionRequest(int count) = 0;

    /**
     * Increase current thread count.
     */
    virtual void IncreaseThreadNum() = 0;

    /**
     * Decrease current thread count.
     */
    virtual void DecreaseThreadNum() = 0;

    /**
     * Reset the internal counter. Reset is needed when {@link IntervalProperty#INTERVAL} or
     * {@link SampleCountProperty#SAMPLE_COUNT} is changed.
     */
    virtual void Reset() = 0;
};

}
}