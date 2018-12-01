#pragma once

#include <cassert>
#include <cstddef>
#include <utility>

template <class Tag>
struct tag_traits;

template <>
struct tag_traits<bool>
{
    static bool is_used(bool b) noexcept
    {
        return b;
    }

    static bool const used_tag = true;
    static bool const unused_tag = false;
};

template <class T, class Tag = bool>
class Tagged_optional
{
public:
    using tag_trait = tag_traits<Tag>;

    explicit Tagged_optional(Tag = tag_trait::unused_tag);
    explicit Tagged_optional(T const&, Tag = tag_trait::used_tag);
    explicit Tagged_optional(T&&, Tag = tag_trait::used_tag);

    Tagged_optional(Tagged_optional const&);
    Tagged_optional& operator=(Tagged_optional const&);

    Tagged_optional(Tagged_optional&&) noexcept;
    Tagged_optional& operator=(Tagged_optional&&) noexcept;

    ~Tagged_optional();

    bool is_used() const noexcept;

private:
    static size_t constexpr payload_size_ = sizeof(T);

    union {
        T payload_;
        char dummy_;
    };
    Tag tag_;
};

template<class T, class Tag>
Tagged_optional<T, Tag>::Tagged_optional(Tag tag)
        : tag_(tag)
        , dummy_()
{
    assert(! is_used());
}

template<class T, class Tag>
Tagged_optional<T, Tag>::Tagged_optional(const T& payload, Tag tag)
        : Tagged_optional(T(payload), tag)
{
    assert(! is_used());
}

template<class T, class Tag>
Tagged_optional<T, Tag>::Tagged_optional(T&& payload, Tag tag)
        : tag_(tag)
        , dummy_()
{
    new (&payload_) T (std::move(payload));
    assert(is_used());
}

template<class T, class Tag>
Tagged_optional<T, Tag>::Tagged_optional(Tagged_optional const& other)
        : tag_(other.tag_)
        , dummy_()
{
    if (is_used()) {
        new (&payload_) T (other.payload_);
    }
}

template<class T, class Tag>
Tagged_optional<T, Tag>&
Tagged_optional<T, Tag>::operator=(Tagged_optional const& other)
{
    if (is_used() && other.is_used()) {
        payload_ = other.payload_;
    } else if (is_used()) {
        payload_.~T();
    } else if (other.is_used()) {
        new (&payload_) T (other.payload_);
    }

    tag_ = other.tag_;

    return *this;
}

template<class T, class Tag>
Tagged_optional<T, Tag>::Tagged_optional(Tagged_optional&& other) noexcept
        : tag_(other.tag_)
        , dummy_()
{
    if (is_used()) {
        new (&payload_) T (std::move(other.payload_));
        other.tag_ = tag_trait::unused_tag;
        other.payload_.~T();
    }
}

template<class T, class Tag>
Tagged_optional<T, Tag>&
Tagged_optional<T, Tag>::operator=(Tagged_optional&& other) noexcept
{
    if (is_used() && other.is_used()) {
        payload_ = std::move(other.payload_);
        other.payload_.~T();
    } else if (is_used()) {
        payload_.~T();
    } else if (other.is_used()) {
        other.payload_.~T();
    }

    tag_ = other.tag_;
    other.tag_ = tag_trait::unused_tag;

    return *this;
}

template<class T, class Tag>
Tagged_optional<T, Tag>::~Tagged_optional()
{
    if (is_used()) {
        payload_.~T();
    }
}

template<class T, class Tag>
bool Tagged_optional<T, Tag>::is_used() const
{
    return tag_trait::is_used(tag_);
}

