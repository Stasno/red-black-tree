#pragma once

#include <iostream>
#include <string>

namespace my
{
	template <typename Tkey, typename Tdata>
	class simple_tree
	{
	private:

		struct node
		{
			Tkey key;
			Tdata data;
			node* left, *right;
			node(Tkey _key, Tdata _data) : key(_key), data(_data)
			{
				left = right = nullptr;
			}
		};
	public:

		void insert(Tkey _key, Tdata _data)
		{
			if (begin == nullptr)
			{
				begin = new node(_key, _data);
				return;
			}

			node* at = begin;

			while (true)
			{
				if (_key <= at->key)
				{
					if (at->left == nullptr)
					{
						at->left = new node(_key, _data);
						return;
					}
					else 
					{
						at = at->left;
					}
				}
				else
				{
					if (at->right == nullptr)
					{
						at->right = new node(_key, _data);
						return;
					}
					else
					{
						at = at->right;
					}
				}
			}
		}

		void print()
		{
			//int deep{deep_rec(begin)};
			rec_print(begin, 0);
		}

	private:

		void rec_print(node *at, int level)
		{
			if (at)
			{
				rec_print(at->right, level + 1);
				for (int i = 0; i < level; i++) std::cout << "   ";
				std::cout << at->key << '\n';
				rec_print(at->left, level + 1);
			}
		}

		//int deep_rec(node* at,int i = 1)
		//{
		//	if (at == nullptr)
		//	{
		//		return i;
		//	}

		//	int a1, a2;
		//	a1 = deep_rec(at->left, i + 1);
		//	a2 = deep_rec(at->right, i + 1);

		//	if (a1 > a2)
		//	{
		//		return a1;
		//	}
		//	else 
		//	{
		//		return a2;
		//	}
		//}

	private:

		node* begin{nullptr};

	};
}