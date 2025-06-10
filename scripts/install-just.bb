#!/usr/bin/env bb
(require '[babashka.process :refer [shell]]
         '[babashka.fs :as fs])

(def version "1.40.0")
(def url (str "https://github.com/casey/just/releases/download/" version
              "/just-" version "-x86_64-unknown-linux-musl.tar.gz"))
(def tmpdir (fs/create-temp-dir))

(defn run [cmd]
  (println cmd)
  (let [{:keys [exit]} (shell {:err :inherit :out :inherit} cmd)]
    (when-not (zero? exit)
      (System/exit exit))))

(try
  (run (format "curl -L %s -o %s/just.tar.gz" url tmpdir))
  (run (format "tar -xzf %s/just.tar.gz -C %s" tmpdir tmpdir))
  (let [dest (fs/path (System/getProperty "user.dir") ".topos" "bin" "just")]
    (fs/create-dirs (fs/parent dest))
    (run (format "install -m755 %s/just %s" tmpdir dest)))
  (finally
    (fs/delete-tree tmpdir)))
