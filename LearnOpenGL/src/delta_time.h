#pragma once
namespace LearnOpenGL {
    class DeltaTime
    {
    public:
        // Static method to access the singleton instance
        static DeltaTime& getInstance() {
            static DeltaTime instance;  // Guaranteed to be created only once
            return instance;
        }

        void Step();

        /// <summary>
        /// Returns the difference between the current frame time and the previous time.
        /// </summary>
        /// <returns></returns>
        float delta_time() const { return delta_time_; }

        /// <summary>
        /// Returns the glfw time in seconds, which is the time since the beginning of the application.
        /// </summary>
        /// <returns></returns>
        float current_frame_time() const{ return current_frame_time_; }

    private:
        // Private constructor to prevent instantiation
        DeltaTime();
        // Delete copy constructor and assignment operator to prevent copying
        DeltaTime(const DeltaTime&) = delete;
        DeltaTime& operator=(const DeltaTime&) = delete;

    private:
        float delta_time_, current_frame_time_;
        float last_frame_time_;
    };
}