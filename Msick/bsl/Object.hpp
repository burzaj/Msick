#pragma once

#include <vector>
#include <string>
#include <tuple>
#include <memory>
#include <functional>
#include <array>

#include "Exceptions.hpp"

namespace bb
{
namespace obj
{
	// class represanting a grid of width and height
	template <typename T>
	class Grid
	{
		std::vector<T> grid = std::vector<T>();
		size_t width = 0;

	public:
		size_t get_height() { return width == 0 ? 0 : grid.size() / width; }
		size_t get_width()  { return width; }
		size_t get_size()   { return grid.size(); }

		size_t get_height() const { return width == 0 ? 0 : grid.size() / width; }
		size_t get_width() const { return width; }
		size_t get_size() const { return grid.size(); }

		std::vector<T>& vector()			 { return grid; }
		const std::vector<T>& vector() const { return grid; }

		T& operator()(size_t _x, size_t _y)				
		{ 
			if (_x >= width or _y * width >= grid.size())
				throw bb::exception::Iterator_out_of_range(bb::errid::Critical + "coordinate/s out of range");
			return grid[_x + _y * width]; 
		}
		const T& operator()(size_t _x, size_t _y) const 
		{
			if (_x >= width or _y * width >= grid.size())
				throw bb::exception::Iterator_out_of_range(bb::errid::Critical + "coordinate/s out of range");
			return grid[_x + _y * width]; 
		}

		T& operator[](size_t _i)						
		{
			if (_i >= grid.size())
				throw bb::exception::Iterator_out_of_range(bb::errid::Critical + "iterator out of range");
			return grid[_i]; 
		}

		const T& operator[](size_t _i) const
		{
			if (_i >= grid.size())
				throw bb::exception::Iterator_out_of_range(bb::errid::Critical + "iterator out of range");
			return grid[_i];
		}

		void resize(size_t _new_width, size_t _new_height, bool _add_padding = false)
		{
			if (_add_padding)
			{
				std::vector<T> new_grid(_new_width * _new_height);

				size_t min_width = std::min(width, _new_width);
				size_t min_height = std::min(get_height(), _new_height);

				for (size_t y = 0; y < min_height; ++y)
				{
					for (size_t x = 0; x < min_width; ++x)
					{
						new_grid[x + y * _new_width] = std::move(grid[x + y * width]);
					}
				}

				grid = std::move(new_grid);
			}
			else
			{
				grid.resize(_new_width * _new_height);
			}

			width = _new_width;
		}

		void clear() {
			grid.clear();
			width = 0U;

			return;
		}

		void fill(const T& val) {
			size_t height = get_height();

			for (size_t y = 0; y < height; ++y)
			{
				for (size_t x = 0; x < width; ++x)
				{
					grid[x + y*width] = val;
				}
			}

			return;
		}

		void fill_rect(size_t _x, size_t _y, size_t w, size_t h, const T& val) {
			for (size_t y = 0; y < h; ++y)
			{
				for (size_t x = 0; x < w; ++x)
				{
					size_t idx = x + _x + (y + _y) * width;

					if (idx >= grid.size())
						throw bb::exception::Iterator_out_of_range(bb::errid::Critical + "iterator out of range");

					grid[idx] = val;
				}
			}

			return;
		}

		bool is_pos_valid(size_t _x, size_t _y) {
			if (_x < 0 or _x >= width or _y < 0 or _y >= get_height())
				return false;
			return true;
		}

		Grid<T> get_rect(size_t _x, size_t _y, size_t w, size_t h) {
			Grid<T> ret_grid(w, h);

			for (size_t y = 0; y < h; ++y)
			{
				for (size_t x = 0; x < w; ++x)
				{
					size_t idx = x + _x + (y + _y) * width;

					if (idx >= grid.size())
						throw bb::exception::Iterator_out_of_range(bb::errid::Critical + "iterator out of range");

					ret_grid(x,y) = grid[idx];
				}
			}

			return ret_grid;
		}

		Grid() = default;

		Grid(size_t _width, size_t _height) : width(_width)
		{
			grid.resize(width * _height);
		}
	};



	 // Grid class with compile time set size
	template <typename T, size_t W, size_t H>
	class StaticGrid {
		std::array<T, W * H> grid;
		size_t width = W;

	public:
		size_t get_height() { return width == 0 ? 0 : grid.size() / width; }
		size_t get_width() { return width; }
		size_t get_size() { return grid.size(); }

		size_t get_height() const { return width == 0 ? 0 : grid.size() / width; }
		size_t get_width() const { return width; }
		size_t get_size() const { return grid.size(); }

		std::array<T, W* H>& array() { return grid; }
		const std::array<T, W* H>& array() const { return grid; }

		T& operator()(size_t _x, size_t _y)
		{
			if (_x > width or _y * width >= grid.size())
				throw bb::exception::Iterator_out_of_range(bb::errid::Critical + "coordinate/s out of range");
			return grid[_x + _y * width];
		}
		const T& operator()(size_t _x, size_t _y) const
		{
			if (_x > width or _y * width >= grid.size())
				throw bb::exception::Iterator_out_of_range(bb::errid::Critical + "coordinate/s out of range");
			return grid[_x + _y * width];
		}

		T& operator[](size_t _i)
		{
			if (_i >= grid.size())
				throw bb::exception::Iterator_out_of_range(bb::errid::Critical + "iterator out of range");
			return grid[_i];
		}

		const T& operator[](size_t _i) const
		{
			if (_i >= grid.size())
				throw bb::exception::Iterator_out_of_range(bb::errid::Critical + "iterator out of range");
			return grid[_i];
		}

		bool is_pos_valid(size_t _x, size_t _y) {
			size_t idx = _x + _y * width;

			if (idx >= grid.size())
				return false;
			return true;
		}

		void fill(const T& val) {
			size_t height = get_height();

			for (size_t y = 0; y < height; ++y)
			{
				for (size_t x = 0; x < width; ++x)
				{
					grid[x + y * width] = val;
				}
			}

			return;
		}

		void fill_rect(size_t _x, size_t _y, size_t w, size_t h, const T& val) {
			for (size_t y = 0; y < h; ++y)
			{
				for (size_t x = 0; x < w; ++x)
				{
					size_t idx = x + _x + (y + _y) * width;

					if (idx > grid.size())
						throw bb::exception::Iterator_out_of_range(bb::errid::Critical + "iterator out of range");

					grid[idx] = val;
				}
			}

			return;
		}

		Grid<T> get_rect(size_t _x, size_t _y, size_t w, size_t h) {
			Grid<T> ret_grid(w, h);

			for (size_t y = 0; y < h; ++y)
			{
				for (size_t x = 0; x < w; ++x)
				{
					size_t idx = x + _x + (y + _y) * width;

					if (idx > grid.size())
						throw bb::exception::Iterator_out_of_range(bb::errid::Critical + "iterator out of range");

					ret_grid(x, y) = grid[idx];
				}
			}

			return ret_grid;
		}

		StaticGrid() = default;
	};
	

	// a template class for implementing any Any class
	template <typename T>
	class Any
	{
		std::shared_ptr<T> pointer;

	public:
		Any() = default;

		explicit Any(std::shared_ptr<T> _ptr) : pointer(std::move(_ptr)) {}

		T* get() { return pointer.get(); }
		const T* get() const { return pointer.get(); }

		void swap(Any& _other) noexcept
		{
			pointer.swap(_other.pointer);
		}

		T* release()
		{
			T* raw = pointer.get();
			pointer.reset();
			return raw;
		}

		Any copy() const
		{
			return Any(pointer);
		}

		Any& operator=(std::shared_ptr<T> _other)
		{
			pointer = std::move(_other);
			return *this;
		}
	};

} // namespace obj
} // namespace bb