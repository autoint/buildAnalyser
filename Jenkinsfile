#!groovy


node {

        echo 'JLOG ==================================== START ===================================='
        echo 'JLOG => Running' + env.BUILD_ID + ' on ' + env.JENKINS_URL + ' for Branch ' + env.BRANCH_NAME
        def workspace = pwd()
        echo 'JLOG => Workspace => ' + workspace
        echo 'JLOG ===================================== END ====================================='

        stage('Checkout') {
               echo 'JLOG => Check out....'
               checkout scm
        }

        stage('Build') { 
                echo 'JLOG => Build here'
                echo 'JLOG => Path = ' + env.PATH
                dir('.\\') {
                   sh 'ls -al'
                   sh 'make'
                }
        }

        stage('Test') {
                echo 'JLOG => Test'
        }

        stage('Deploy') {
                echo 'Deploy'
                if (currentBuild.result == 'SUCCESS') {
                    sh 'make publish'
                }
        }

        stage ('Create build output') {

                // Make the output directory.
                sh 'mkdir -p output'

                // Write an useful file, which is needed to be archived.
                writeFile file: 'output/usefulfile.txt', text: 'This file is useful, need to archive it.'

                // Write an useless file, which is not needed to be archived.
                writeFile file: 'output/uselessfile.md', text: 'This file is useless, no need to archive it.'
        }
 
       stage ('Archive build output') {

                // Archive the build output artifacts.
                archiveArtifacts artifacts: 'output/*.txt', excludes: 'output/*.md'

       }

}

