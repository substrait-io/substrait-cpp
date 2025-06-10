#!/usr/bin/env bb
(require '[babashka.process :refer [shell]]
         '[babashka.fs :as fs])

(def work-dir (System/getProperty "user.dir"))
(def bin-dir (fs/path work-dir ".topos" "bin"))
(fs/create-dirs bin-dir)

(shell {:extra-env {"JUST_DEST" (str bin-dir "/just")}
        :out :inherit :err :inherit}
       "bb" "scripts/install-just.bb")

(def env {"PATH" (str bin-dir ":" (System/getenv "PATH"))})
(shell {:out :inherit :err :inherit} "git" "submodule" "update" "--init" "--recursive")
(shell {:extra-env env :out :inherit :err :inherit} "just" "tests")
