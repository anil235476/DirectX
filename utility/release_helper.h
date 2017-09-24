#ifndef __RELEASE_HELPER_H_
#define __RELEASE_HELPER_H_

namespace util {
	template<class T>
	class release_helper {
	public:
		release_helper() = default;
		release_helper(T* p):p_{p}{}
		release_helper(const release_helper&) = delete;
		release_helper& operator=(const release_helper&) = delete;
		~release_helper() {
			if (p_)
				p_->Release();
			p_ = nullptr;
		}

		T* operator->() { return get(); }
		T** ref() { return &p_; };
		T* get() {
			return p_;
		}

		operator bool() {
			return p_ != nullptr;
		}


	private:
		T* p_{ nullptr };
	};
}

#endif//__RELEASE_HELPER_H_