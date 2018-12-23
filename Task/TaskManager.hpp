/*
This Source Code Form is subject to the
terms of the Mozilla Public License, v.
2.0. If a copy of the MPL was not
distributed with this file, You can
obtain one at
http://mozilla.org/MPL/2.0/.
*/
/** @file
 * 
 */

#ifndef TASK_MANAGER_04_02_19
#define TASK_MANAGER_04_02_19
#pragma once

namespace Task
{
    template< typename T = void, typename ...Args >
    class Task
    {
      public:
        Task() = default;
        virtual ~Task() = default;
        virtual void run( T, Args... ) =0;
    };

    class OpenGlTask : public Task<void>
    {
      public:
        OpenGlTask( unsigned OpenGlContextID )
                : OpenGlContextID { OpenGlContextID } {};
        virtual ~OpenGlTask() = default;

        void run()
        {

        };


      private:
        const unsigned OpenGlContextID;
    };

    class Manager
    {
        friend auto getManager() -> Manager&;
      public:

      private:
        Manager();
    };

    auto getManager() -> Manager&
    {
        static Manager m;
        return m;
    }
}

#endif // !TASK_MANAGER_04_02_19
