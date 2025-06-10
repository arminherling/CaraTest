#include "TestSuite.h"

namespace CaraTest
{
    TestSuite::TestSuite(const QString& name)
        : m_name{ name }
        , m_tests{}
    {
    }
}
