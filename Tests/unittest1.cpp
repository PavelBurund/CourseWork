#include "stdafx.h"
#include "CppUnitTest.h"
#include <string>
#include "../Ford_Falk/Graph.h"
#include "../Ford_Falk/Graph.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Test
{
	TEST_CLASS(UnitTest1)
	{
	public:

		TEST_METHOD(Empty_file)
		{
			try
			{
				Graph exp;

				exp.take_file("C:/Users/LENOVO/source/repos/Ford_Falk/Ford_Falk/emptyfile.txt");
			}
			catch (const invalid_argument error)
			{
				Assert::AreEqual("Файл пустой!!", error.what());
			}
		}

		TEST_METHOD(incorrect_channel_width)
		{
			try
			{
				Graph exp;

				exp.take_file("C:/Users/LENOVO/source/repos/Ford_Falk/Ford_Falk/incorrect_channel_width.txt");
			}
			catch (const invalid_argument error)
			{
				Assert::AreEqual("Пропускная способность не может быть меньше 0", error.what());
			}
		}

		TEST_METHOD(Loop_in_file)
		{
			try
			{
				Graph exp;

				exp.take_file("C:/Users/LENOVO/source/repos/Ford_Falk/Ford_Falk/loop_.txt");

			}
			catch (const invalid_argument error)
			{
				Assert::AreEqual("Начальная и конечная точки равны", error.what());
			}
		}

		TEST_METHOD(Do_not_have_S)
		{
			try
			{
				Graph exp;

				exp.take_file("C:/Users/LENOVO/source/repos/Ford_Falk/Ford_Falk/Do_not_have_S.txt");

			}
			catch (const logic_error error)
			{
				Assert::AreEqual("Нет начала!", error.what());
			}
		}

		TEST_METHOD(Do_not_have_T)
		{
			try
			{
				Graph exp;

				exp.take_file("C:/Users/LENOVO/source/repos/Ford_Falk/Ford_Falk/Do_not_have_T.txt");

			}
			catch (const logic_error error)
			{
				Assert::AreEqual("Нет конца!", error.what());
			}
		}

		TEST_METHOD(Way_in_S)
		{
			try
			{
				Graph exp;

				exp.take_file("C:/Users/LENOVO/source/repos/Ford_Falk/Ford_Falk/Way_in_S.txt");

			}
			catch (const invalid_argument error)
			{
				Assert::AreEqual("Неправильный формат ввода", error.what());
			}
		}

		TEST_METHOD(Way_from_T)
		{
			try
			{
				Graph exp;

				exp.take_file("C:/Users/LENOVO/source/repos/Ford_Falk/Ford_Falk/Way_from_T.txt");

			}
			catch (const invalid_argument error)
			{
				Assert::AreEqual("Неправильный формат ввода", error.what());
			}
		}

		TEST_METHOD(really_way)
		{
			try
			{
				Graph exp;

				exp.take_file("C:/Users/LENOVO/source/repos/Ford_Falk/Ford_Falk/really_way.txt");

			}
			catch (const invalid_argument error)
			{
				Assert::AreEqual("Тут никакой ошибки нет!!", error.what());
			}
		}
	};
}