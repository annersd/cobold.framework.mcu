#pragma once

#include "Component.h"
#include <functional>
#include "Object.h"

namespace cobold::components
{

    /**
     * @class DynamicComponent
     * @brief The DynamicComponent class is a base class for physical components that require
     *        update and initialization behavior.
     */
    template <typename TBoundObject>
    class DynamicComponent : public Component
    {
    private:
        std::function<void(DynamicComponent<TBoundObject> *)> initialize_func = nullptr;
        std::function<void(DynamicComponent<TBoundObject> *)> update_func = nullptr;
        std::function<void(DynamicComponent<TBoundObject> *)> configure_func = nullptr;

    public:
        cobold::sys::Object<TBoundObject> *bound_object;

        /**
         * @brief Construct a new DynamicComponent object
         */
        DynamicComponent() = default;

        /**
         * @brief Destroy the DynamicComponent object
         */
        virtual ~DynamicComponent() = default;

        virtual void initialize_impl() override
        {
            if (initialize_func != nullptr)
                initialize_func(this);
        }

        virtual void update_impl() override
        {
            if (update_func != nullptr)
                update_func(this);
        }

        virtual void configure_impl() override
        {
            if (configure_func != nullptr)
                configure_func(this);
        }

        void bind_initialize(std::function<void(DynamicComponent<TBoundObject> *)> initialize)
        {
            this->initialize_func = initialize;
        }

        void bind_update(std::function<void(DynamicComponent<TBoundObject> *)> update)
        {
            this->update_func = update;
        }

        void bind_configure(std::function<void(DynamicComponent<TBoundObject> *)> configure)
        {
            this->configure_func = configure;
        }
    };

} // namespace cobold::components