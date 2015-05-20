#include "sx1276.hpp"
#include "spi_factory.hpp"
#include "misc.h"
#include <boost/shared_ptr.hpp>
#include <boost/format.hpp>

using boost::shared_ptr;
using boost::format;
using std::cout;

inline std::string safe_str(const char *m)
{
	std::string result;
	for (; *m; ) { char c = *m++; result += iscntrl(c) ? '.' : c; }
	return result;
}

int main(int argc, char* argv[])
{
  if (argc < 2) { fprintf(stderr, "Usage: %s <spidev>", argv[0]); return 1; }

  shared_ptr<SPI> spi = SPIFactory::GetInstance(argv[1]);
  if (!spi) { PR_ERROR("Unable to create SPI device instance\n"); return 1; }

  Misc::UserTraceSettings(spi);

  // TODO work out how to run without powering off / resetting the module

  usleep(100);
  SX1276Radio radio(spi);

  cout << format("SX1276 Version: %.2x\n") % radio.QueryVersion();

  radio.ApplyDefaultLoraConfiguration();

	if (radio.fault()) return 1;

	const char *msg = "Hello, World!\n";
	printf("Beacon message: '%s'\n", safe_str(msg).c_str());
	printf("Predicted time on air: %fs\n", radio.PredictTimeOnAir(msg));

	long total = 0;
	long faultCount = 0;
  while (true) {
		total++;
    if (radio.SendSimpleMessage(msg)) { printf("."); fflush(stdout); /*radio.StandbyMode(); usleep(200000); */ continue; }
		radio.StandbyMode();
		printf("\n");
		faultCount++;
		PR_ERROR("Fault on send detected: %ld of %ld\n", faultCount, total);
		printf("Beacon message: '%s'\n", safe_str(msg).c_str());
		printf("Predicted time on air: %fs\n", radio.PredictTimeOnAir(msg));
		spi->AssertReset();
	  radio.ApplyDefaultLoraConfiguration();
		usleep(2000000);
  }
  return 1;
}
