#include "pch.h"
#include "CppUnitTest.h"
#include "../Program/main.cpp"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{
	TEST_CLASS(UnitTest)
	{
	public:
		
		TEST_METHOD(met_get_predok)
		{
			Node* node = new Node();
			node->data = 1;
			node->left = new Node();
			node->left->data = 2;
			node->right = new Node();
			node->right->data = 3;
			int findlevel;
			Node* g = get_predok(node, 2, 3, new bool[2]{ false, false }, findlevel);
			Assert::AreEqual(findlevel, 1);
			Assert::AreEqual(g->data, 1);
		}
	};
}
