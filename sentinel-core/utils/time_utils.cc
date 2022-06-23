#include "time_utils.h"

/**
 * <p>Provides millisecond-level time of OS.</p>
 * <p>
 * Here we should see that not all the time TimeUtil should
 * keep looping 1_000 times every second (Actually about 800/s due to some losses).
 * <pre>
 * * In idle conditions it just acts as System.currentTimeMillis();
 * * In busy conditions (significantly more than 1_000/s) it keeps loop to reduce costs.
 * </pre>
 * For detail design and proposals please goto
 * <a href="https://github.com/alibaba/Sentinel/issues/1702#issuecomment-692151160">https://github.com/alibaba/Sentinel/issues/1702</a>
 * @author alibaba.com
 * @author huangshiyi
 * */

namespace Sentinel {
namespace Utils {

std::chrono::milliseconds TimeUtils::CurrentTimeMillis() {
  return std::chrono::duration_cast<std::chrono::milliseconds>(
      std::chrono::system_clock::now().time_since_epoch());
}

void TimeUtils::Run()
{
  while(true)
  {
    this->Check();
  }
}

void TimeUtils::Check()
{
  long now = this->CurrentTime(true);
  //every period
  if(now - this->last_check_ < CHECK_INTERVAL_)
  {
    return ;
  }
  this->last_check_ = now;


}

}  // namespace Utils
}  // namespace Sentinel
