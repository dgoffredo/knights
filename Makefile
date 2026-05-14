knights: knights.cpp
	$(CXX) -o $@ -O3 -Wall -Wextra -pedantic --std=c++20 $<

%.png: %.tab plotpng plotcmd
	./plotpng "$<"

%.tab: knights getcolors getknights
	./knights "$$(./getcolors '$@')" | head "-$$(./getknights "$@")" >'$@'

