#include <unistd.h>

int main()
{
	write(3, "a", 1);
	return 0;
}
