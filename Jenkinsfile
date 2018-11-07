pipeline {
  agent {
    node {
      label 'linux ubuntu'
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
        sh '''mkdir -p output

writeFile file: \'output/usefulfile.txt\', text: \'This file is useful, need to archive it.\'

writeFile file: \'output/uselessfile.md\', text: \'This file is useless, no need to archive it.\''''
      }
    }
    stage('Archive build output') {
      steps {
        archiveArtifacts(artifacts: 'output/*.txt', allowEmptyArchive: true, excludes: 'output/*.md')
      }
    }
  }
}