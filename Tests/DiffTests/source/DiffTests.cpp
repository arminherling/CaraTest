#include <CaraTest.h>
#include <CaraTest/Diff.h>
#include <tuple>

using namespace CaraTest;

static void DiffTest(const std::string& first, const std::string& second, const std::vector<DiffLocation>& expectedDifferences)
{
    const auto differences = diff(first, second);

    areEqual(expectedDifferences.size(), differences.size());
    for (auto i = 0; i < differences.size(); i++)
    {
        areEqual(expectedDifferences.at(i).startIndex, differences.at(i).startIndex);
        areEqual(expectedDifferences.at(i).endIndex, differences.at(i).endIndex);
    }
}

static std::vector<std::tuple<std::string, std::string, std::vector<DiffLocation>>> DiffTest_Data()
{
    return {
        std::make_tuple(std::string(), std::string(), std::vector<DiffLocation>()),
        std::make_tuple(std::string(""), std::string(""), std::vector<DiffLocation>()),
        std::make_tuple(std::string("x"), std::string("x"), std::vector<DiffLocation>()),
        std::make_tuple(std::string("x"), std::string("y"), std::vector<DiffLocation>{
            DiffLocation{.startIndex = 0, .endIndex = 0, .change = DiffChange::Deletion},
            DiffLocation{.startIndex = 0, .endIndex = 0, .change = DiffChange::Addition}
        }),
        std::make_tuple(std::string("abc123"), std::string("abc789"), std::vector<DiffLocation>{
            DiffLocation{.startIndex = 3, .endIndex = 3, .change = DiffChange::Deletion},
            DiffLocation{.startIndex = 4, .endIndex = 4, .change = DiffChange::Deletion},
            DiffLocation{.startIndex = 5, .endIndex = 5, .change = DiffChange::Deletion},
            DiffLocation{.startIndex = 3, .endIndex = 3, .change = DiffChange::Addition},
            DiffLocation{.startIndex = 4, .endIndex = 4, .change = DiffChange::Addition},
            DiffLocation{.startIndex = 5, .endIndex = 5, .change = DiffChange::Addition}
    }),
        std::make_tuple(
            std::string("enum Values \n{\n    First\n    Second\n    Third\n}\n"),
            std::string("enum Values \n{\n    First\n    Second = 3\n    Third\n}\n"),
            std::vector<DiffLocation>{
                DiffLocation{.startIndex = 35, .endIndex = 35, .change = DiffChange::Addition},
                DiffLocation{.startIndex = 36, .endIndex = 36, .change = DiffChange::Addition},
                DiffLocation{.startIndex = 37, .endIndex = 37, .change = DiffChange::Addition},
                DiffLocation{.startIndex = 38, .endIndex = 38, .change = DiffChange::Addition}
            }),
        std::make_tuple(
std::string(R"(enum Values 
{
    First
    Second
    Third
}
)"),
std::string(R"(enum Values 
{
    First
    Second = 3
    Third
}
)"),
    std::vector<DiffLocation>{
        DiffLocation{.startIndex = 35, .endIndex = 35, .change = DiffChange::Addition},
        DiffLocation{.startIndex = 36, .endIndex = 36, .change = DiffChange::Addition},
        DiffLocation{.startIndex = 37, .endIndex = 37, .change = DiffChange::Addition},
        DiffLocation{.startIndex = 38, .endIndex = 38, .change = DiffChange::Addition}
    }),
    };
}

auto diffSuite = CaraTest::getSuite("DiffTests");
auto diffTest1 = diffSuite->add("DiffTest", DiffTest, DiffTest_Data);
