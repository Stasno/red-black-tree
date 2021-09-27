#pragma once

#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>

#include <cassert>

// memory leak viewer
#ifdef _DEBUG
#define new new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
// Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
// allocations to be of _CLIENT_BLOCK type
#else
#define DBG_NEW new
#endif

namespace my {

	template <typename Tf, typename Ts>
	struct pair
	{
		Tf first;
		Ts second;

		pair()
		{

		}

		pair(Tf _f, Ts _s) : first(_f), second(_s)
		{

		}

		pair& operator=(pair&&) = default;

		pair(const pair&) = default;
		pair(pair&&) = default;

	};

	template <typename Tkey, typename Tdata>
	class red_black_tree
	{
	private:

		class iterator;

		enum Tcolor
		{
			black,
			red
		};

		struct node
		{
		public:
			pair<Tkey, Tdata> value;
			Tcolor	color;

			bool isNil = false;

			node* left{ nullptr },
				* right{ nullptr },
				* parent{ nullptr };

			~node()
			{
				if (this->isNil)
					return;

				parent == nullptr;
				if (!left->isNil)
				{
					delete left;
					left = nullptr;
				}
				if (!right->isNil)
				{
					delete right;
					right = nullptr;
				}
			}

		public:

			node()
			{
				isNil = true;
			}

			node(Tkey _key,
				Tdata _data,
				node* _parent,
				node* nil_node,
				Tcolor _color = Tcolor::red) :
					value(_key, _data),
					parent(_parent),
					color(_color)
			{
				left = nil_node;
				right = nil_node;
			}

			Tkey key()
			{
				return value.first;
			}

			node* grandparent()
			{
				//if (this->parent != nullptr)
				if (!this->parent->isNil)
					return parent->parent;
				else
					return this->parent;
			}

			node* uncle()
			{
				node* buffer = grandparent();

				//if (buffer == nullptr)
				if (buffer->isNil)
					return buffer;

				if (this->parent == buffer->left)
					return buffer->right;
				else
					return buffer->left;
			}

			node* brother()
			{
				if (this->parent->left == this)
					return this->parent->right;
				else
					return this->parent->left;
			}

			void replace_with(node* with)
			{
				//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

				this->value = std::move(with->value);
			}

			bool is_leaf()
			{
				return left->isNil && right->isNil;
			}

			static bool is_black(node* n)
			{
				//if (n == nullptr)
				if (n->isNil)
					return true;
				else
					if (n->color == black)
						return true;
				return false;
			}

		};

	private:
		using nodeptr = node*;
		//pointer to the top of the tree
		nodeptr head{ nullptr };

		// nil node replaces nullptr as end point node
		// ! parent ptr is the head of the tree
		// ! left ptr is the node with the lowest value
		// ! right ptr is the node with the highest value
		nodeptr nil_node = new node();
		
		size_t _size{};
		

	public:

		~red_black_tree() noexcept
		{
			if (head != nullptr)
			{
				delete head;
				head = nullptr;
				_size = 0;
			}

			if (nil_node != nullptr)
			{
				delete nil_node;
				nil_node = nullptr;
			}

		}

		void insert(Tkey _key, Tdata _data) noexcept
		{
			if (head == nullptr)
			{
				head = new node(_key, _data, nil_node, nil_node, Tcolor::black);
				nil_node->parent = head;
				return;
			}

			nodeptr at = head;
			while (true)
			{
				// // //
				if (_key == at->key())
				{
					return;
				}

				if (_key < at->key())
				{
					if (at->left->isNil)
					{
						at->left = new node(_key, _data, at, nil_node, Tcolor::red);
						at = at->left;
						break;
					}
					else
						at = at->left;
				}
				else
				{
					if (at->right->isNil)
					{
						at->right = new node(_key, _data, at, nil_node, Tcolor::red);
						at = at->right;
						break;
					}
					else
						at = at->right;
				}
			}
			_size++;
			balance(at);

		}

		void erase(Tkey _key) noexcept
		{
			nodeptr at = head;
			while (at->key() != _key)
			{
				if (_key < at->key())
				{
					at = at->left;
				}
				else
				{
					at = at->right;
				}

				if (at->isNil)
					return;

			}

			/*
			* if current node is an leaf
			*/
			if (at->is_leaf())
			{
				remove_node(at);
				return;
			}
			else
			{
				/*
				* if the left leaf doesn't exist
				* then remove the right leaf
				*/
				if (at->left->isNil)
				{
					at->replace_with(at->right);
					remove_node(at->right);
					return;
				}

				/*
				* if the left leaf doesn't exist 
				* then remove the right leaf
				*/
				if (at->right->isNil)
				{
					at->replace_with(at->left);
					remove_node(at->left);
					return;
				}

				/*
				* if both leafs exists 
				* then look for the largest node in the left subtree and remove
				*/
				nodeptr max;
				
				max = at->left;
				while (true)
				{
					if (!max->right->isNil)
						max = max->right;
					else
						if (!max->left->isNil)
							max = max->left;
						else
							break;
				}

				at->replace_with(max);
				remove_node(max);
				return;

			}

		}

		iterator find(Tkey _key) const noexcept
		{
			nodeptr at = head;
			while (!at->isNill)
			{
				if (_key == at->key())
				{
					return iterator(at);
				}
				if (_key < at->key())
				{
					at = at->left;
				}
				else
				{
					at = at->right;
				}
			}
			return iterator(nullptr);
		}

		void clear() noexcept
		{
			if (begin != nullptr)
			{
				delete begin;
				begin = nullptr;
				_size = 0;
			}
		}

		size_t size()
		{
			return _size;
		}

		void print(std::map<int, int>::_Nodeptr ptr) noexcept
		{
			cout << "\nMy tree \n--\n";
			rec_print(head, 0);
			cout << "\n--\n Stl map tree \n--\n";
			map_rec_print(ptr, 0);
			cout << "-----------" << '\n';
		}

	private:

		void rec_print(nodeptr at, int level)
		{
			if (!at->isNil)
			{
				rec_print(at->right, level + 1);
				for (int i = 0; i < level; i++) std::cout << "   ";
				char a;
				if (at->color == black)
					a = 'b';
				else
					a = 'r';
				std::cout << at->key() << a << '\n';
				rec_print(at->left, level + 1);
			}
		}


		void map_rec_print(std::map<int, int>::_Nodeptr at, int level)
		{
			if (!at->_Isnil)
			{
				map_rec_print(at->_Right, level + 1);
				for (int i = 0; i < level; i++) std::cout << "   ";
				char a;
				if (at->_Color == 1)
					a = 'b';
				else
					a = 'r';
				std::cout << at->_Myval.first << a << '\n';
				map_rec_print(at->_Left, level + 1);
			}
		}

	private:

		/*
		* erase operations
		*/

		void remove_node(nodeptr n)
		{
			if (n->color == black)
				remove_step1(n);
			if (n == n->parent->left)
				n->parent->left = nil_node;
			else
				n->parent->right = nil_node;
			delete n;
			_size--;
		}

		void remove_step1(nodeptr n)
		{
			if (!n->parent->isNil)
				remove_step2(n);
		}

		void remove_step2(nodeptr n)
		{
			nodeptr b = n->brother();

			if (node::is_black(b) == false)
			{
				n->parent->color = red;
				b->color = black;
				if (n == n->parent->left)
					rotate_left(n->parent);
				else
					rotate_right(n->parent);
			}
			remove_step3(n);
		}

		void remove_step3(nodeptr n)
		{
			nodeptr b = n->brother();
			if (n->parent->color == black && b->color == black &&
				node::is_black(b->left) == true && node::is_black(b->right) == true)
			{
				b->color = red;
				remove_step1(n->parent);
			}
			else
			{
				remove_step4(n);
			}
		}

		void remove_step4(nodeptr n)
		{
			nodeptr b = n->brother();

			if (n->parent->color == red && b->color == black &&
				node::is_black(b->left) == true && node::is_black(b->right) == true)
			{
				b->color = red;
				n->parent->color = black;
			}
			else
			{
				remove_step5(n);
			}
		}

		void remove_step5(nodeptr n)
		{
			nodeptr b = n->brother();
			if (b->color == black)
			{
				if (n == n->parent->left && 
					b->right->color == black &&
					b->left->color == red)
				{
					b->color = red;
					b->left->color = black;
					rotate_right(b);
				}
				else
				{
					if (n == n->parent->right &&
						b->left->color == black &&
						b->right->color == red)
					{
						b->color = red;
						b->right->color = black;
						rotate_left(b);
					}
				}
				remove_step6(n);
			}
		}

		void remove_step6(nodeptr n)
		{
			nodeptr b = n->brother();

			b->color = n->parent->color;
			n->parent->color = black;
			if (n == n->parent->left)
			{
				b->right->color = black;
				rotate_left(n->parent);
			}
			else
			{
				b->left->color = black;
				rotate_right(n->parent);
			}
		}

	private:

		/*
		* insert operations
		*/

		void balance(nodeptr n)
		{
			insert_step1(n);
		}

		void insert_step1(nodeptr n)
		{
			if (n->parent->isNil)
				n->color = black;
			else
				insert_step2(n);
		}

		void insert_step2(nodeptr n)
		{
			if (n->parent->color == black)
				return;
			else
				insert_step3(n);
		}

		void insert_step3(nodeptr n)
		{
			nodeptr u = n->uncle();
			nodeptr g;

			if (!u->isNil && u->color == red)
			{
				n->parent->color = black;
				u->color = black;
				g = n->grandparent();
				g->color = red;
				insert_step1(g);
			}
			else
				insert_step4(n);
		}

		void insert_step4(nodeptr n)
		{
			nodeptr g = n->grandparent();
			if (n == n->parent->right && n->parent == g->left)
			{
				rotate_left(n->parent);
				n = n->left;
			}
			else
			{
				if (n == n->parent->left && n->parent == g->right)
				{
					rotate_right(n->parent);
					n = n->right;
				}
			}
			insert_step5(n);
		}

		void insert_step5(nodeptr n)
		{
			nodeptr g = n->grandparent();
			n->parent->color = black;
			g->color = red;
			if (n == n->parent->left && n->parent == g->left)
				rotate_right(g);
			else
				rotate_left(g);
			//insert_step1(n);
		}

		/*
		* end of insert operations
		*/

	private:

		void rotate_left(nodeptr n)
		{
			nodeptr pivot = n->right;
			
			pivot->parent = n->parent;
			if (!n->parent->isNil)
			{
				if (n->parent->left == n)
					n->parent->left = pivot;
				else
					n->parent->right = pivot;
			}
			else
			{
				head = pivot;
				nil_node->parent = head;
			}

			n->right = pivot->left;
			if (!pivot->left->isNil)
				pivot->left->parent = n;

			n->parent = pivot;
			pivot->left = n;
		}

		void rotate_right(nodeptr n)
		{
			nodeptr pivot = n->left;

			pivot->parent = n->parent;
			if (!n->parent->isNil)
			{
				if (n->parent->left == n)
					n->parent->left = pivot;
				else
					n->parent->right = pivot;
			}
			else
			{
				head = pivot;
				nil_node->parent = head;
			}

			n->left = pivot->right;
			if (!pivot->right->isNil)
				pivot->right->parent = n;

			n->parent = pivot;
			pivot->right = n;
		}

		//iterators
		public:

			class iterator
			{
			public:

				nodeptr ptr; //pointer to node

			public:

				iterator& operator++() noexcept
				{
					increment();
					return *this;
				}

				iterator& operator++(int) noexcept
				{
					auto out = *this;
					increment();
					return out;
				}

				iterator& operator--() noexcept
				{
					decrement();
					return *this;
				}

				iterator& operator--(int) noexcept
				{
					auto out = *this;
					decrement();
					return out;
				}

				bool operator!=(const iterator& right) const noexcept
				{
					return ptr != right.ptr;
				}

				bool operator==(const iterator& right) const noexcept
				{
					return ptr == right.ptr;
				}

				pair<Tkey, Tdata>* operator->()
				{
					assert(!ptr->isNil && "cannot dereference end map/set iterator");
					return &ptr->value;
				}

				pair<Tkey, Tdata>& operator*()
				{
					assert(!ptr->isNil && "cannot dereference end map/set iterator");
					return ptr.value;
				}

				iterator(nodeptr _ptr)
				{
					ptr = _ptr;
				}

			private:

				void increment()
				{
					if (ptr->right->isNil)
					{
						nodeptr n;
						while (true)
						{
							n = ptr->parent;
							if (!n->isNil && ptr == n->right)
								ptr = n;
							else
								break;
						}
						ptr = n;
					}
					else
					{
						ptr = ptr->right;
						while (!ptr->left->isNil)
							ptr = ptr->left;
					}

				}

				void decrement()
				{
					if (ptr->isNil)
					{
						ptr = ptr->right;
						return;
					}

					if (ptr->left->isNil)
					{
						nodeptr n;
						while (true)
						{
							n = ptr->parent;
							if (!n->isNil && ptr == n->left)
								ptr = n;
							else
								break;
						}

						if (!ptr->isNil) { // decrement non-begin()
							ptr = n; // ==> parent if not head
						}
					}
					else
					{
						ptr = ptr->left;
						while (!ptr->right->isNil)
							ptr = ptr->right;
					}
				}

			};

		private:

			nodeptr get_Nilnode()
			{
				nodeptr buf = head;
				while (!buf->left->isNil)
					buf = buf->left;

				nil_node->left = buf;

				buf = head;

				while (!buf->right->isNil)
					buf = buf->right;

				nil_node->right = buf;

				return nil_node;
			}

		public:

			iterator begin() noexcept
			{
				return iterator(get_Nilnode()->left);
			}

			iterator end() noexcept
			{
				return iterator(get_Nilnode());
			}

			iterator rbegin() noexcept
			{
				return iterator(get_Nilnode()->right);
			}

			iterator rend() noexcept
			{
				return iterator(get_Nilnode());
			}


	};// end class red_black_tree
}// namespace my

