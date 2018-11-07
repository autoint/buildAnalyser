pipeline {
  agent {
    node {
      label 'LIN_WKR'
    }

  }
  stages {
    stage('Infrastructure') {
      steps {
        echo 'Setup Tools'
        sh 'sudo apt-get -y install gcc make build-essential cmake'
        echo 'Create Storage Areas'
        sh 'mkdir output'
      }
    }
    stage('Build') {
      steps {
        sh 'make > output/build.txt 2>&1'
      }
    }
    stage('Test') {
      steps {
        echo 'Testing 1 2 3'
        sh 'touch output/test.txt'
      }
    }
    stage('Deploy') {
      steps {
        sh 'make publish > output/publish.txt 2>&1'
      }
    }
    stage('Create build output') {
      steps {
        // Make the output directory.
        sh 'mkdir -p output'

        // Write an useful file, which is needed to be archived.
        writeFile file: 'output/usefulfile.txt', text: 'This file is useful, need to archive it.'

        // Write an useless file, which is not needed to be archived.
        writeFile file: 'output/uselessfile.md', text: 'This file is useless, no need to archive it.'
      }
    }
    stage('Archive build output') {
      steps {
        archiveArtifacts(artifacts: 'output/*.txt', allowEmptyArchive: true, excludes: 'output/*.md')
      }
    }
  }
}