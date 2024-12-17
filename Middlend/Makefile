SANITIZER = -fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,leak,nonnull-attribute,null,object-size,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr
DIFFERENT_FLAGS = -I include -D _DEBUG -lncurses -ggdb3 -std=c++17 -O0 -Wall -Wextra -Weffc++ -Waggressive-loop-optimizations -Wc++14-compat -Wmissing-declarations -Wcast-align -Wcast-qual -Wchar-subscripts -Wconditionally-supported -Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal -Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 -Winline -Wlogical-op -Wnon-virtual-dtor -Wopenmp-simd -Woverloaded-virtual -Wpacked -Wpointer-arith -Winit-self -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=2 -Wsuggest-attribute=noreturn -Wsuggest-final-methods -Wsuggest-final-types -Wsuggest-override -Wswitch-default -Wswitch-enum -Wsync-nand -Wundef -Wunreachable-code -Wunused -Wuseless-cast -Wvariadic-macros -Wno-literal-suffix -Wno-missing-field-initializers -Wno-narrowing -Wno-old-style-cast -Wno-varargs -Wstack-protector -fcheck-new -fsized-deallocation -fstack-protector -fstrict-overflow -flto-odr-type-merging -fno-omit-frame-pointer -Wlarger-than=81920 -Wstack-usage=81920 -pie -fPIE -Werror=vla
OBJECTS = main.o \
			../General/objects/wrDotFile.o ../General/objects/wrPngFile.o ../General/objects/choose_color_and_name_byType.o \
			../General/objects/treeFuncs.o \
			../General/objects/pushTree.o ../General/objects/pullTree.o

all: out

out: $(OBJECTS)
	g++ $(OBJECTS) -o out $(DEBUG_SET)  $(DIFFERENT_FLAGS) $(SANITIZER)

main.o: main.cpp
	g++ -c main.cpp -o main.o $(DEBUG_SET)  $(DIFFERENT_FLAGS) $(SANITIZER)


# ---------------------------------------- graphDump --------------------------------------------- 
../General/objects/wrDotFile.o: ../General/graphDump/wrDotFile.cpp
	g++ -c ../General/graphDump/wrDotFile.cpp -o ../General/objects/wrDotFile.o $(DEBUG_SET)  $(DIFFERENT_FLAGS) $(SANITIZER)

../General/objects/wrPngFile.o: ../General/graphDump/wrPngFile.cpp
	g++ -c ../General/graphDump/wrPngFile.cpp -o ../General/objects/wrPngFile.o $(DEBUG_SET)  $(DIFFERENT_FLAGS) $(SANITIZER)

../General/objects/choose_color_and_name_byType.o: ../General/graphDump/choose_color_and_name_byType.cpp
	g++ -c ../General/graphDump/choose_color_and_name_byType.cpp -o ../General/objects/choose_color_and_name_byType.o $(DEBUG_SET)  $(DIFFERENT_FLAGS) $(SANITIZER)

# ---------------------------------------- programTree -------------------------------------------
../General/objects/treeFuncs.o: ../General/programTree/treeFuncs.cpp
	g++ -c ../General/programTree/treeFuncs.cpp -o ../General/objects/treeFuncs.o $(DEBUG_SET)  $(DIFFERENT_FLAGS) $(SANITIZER)

# ---------------------------------------- treeTransfer -------------------------------------------
../General/objects/pushTree.o: ../General/treeTransfer/pushTree.cpp
	g++ -c ../General/treeTransfer/pushTree.cpp -o ../General/objects/pushTree.o $(DEBUG_SET)  $(DIFFERENT_FLAGS) $(SANITIZER)

../General/objects/pullTree.o: ../General/treeTransfer/pullTree.cpp
	g++ -c ../General/treeTransfer/pullTree.cpp -o ../General/objects/pullTree.o $(DEBUG_SET)  $(DIFFERENT_FLAGS) $(SANITIZER)

clean:
	rm -rf *.o out
